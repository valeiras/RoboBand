#ifndef Percu_controller_h
#define Percu_controller_h

#include <Array.h>
#include <bpm_values.h>
#include <percu_robot.h>
#include <percu_song.h>
#include <robo_receptor.h>

#include "Arduino.h"

#define POS_SECURITY_FACTOR 0.4

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
class PercuController : public RoboReceptor {
 public:
  PercuController(PercuRobot<NB_HIT_JOINTS, NB_POS_JOINTS> *robot, PercuSong<NB_HIT_JOINTS, BITS_FOR_POS> *song,
                  int address, bool isSimulation, bool hasOutput);

  void initialize(unsigned long currTime);

  virtual int goToTime(unsigned long currTime, bool hasOutput = 0);

  void manageHitInstruction(byte limb, unsigned long currTime);
  void managePosInstruction(byte limb);

  bool isTimeToChangePos(byte limb, unsigned long ellapsedTime);

  void setBpm(uint8_t bpm);
  void setBpm(float bpm);
  void setTimePerSemiquaver(unsigned long timePerSemiquaver);

  signed int getResyncCompensationTime();

  // Inherited methods from RoboReceptor
  virtual void processStartMsg();
  virtual void processStartSongMsg(uint8_t messageContent);
  virtual void processStopMsg();
  virtual void processResyncMsg();
  virtual void processBpmChangeMsg(uint8_t messageContent);
  virtual void processBpmIdxChangeMsg(uint8_t messageContent);
  virtual void processLimbStopMsg(uint8_t messageContent);
  virtual void processLimbStartMsg(uint8_t messageContent);
  virtual void processNoteOnMsg(uint8_t messageContent);
  virtual void processNoteOffMsg(uint8_t messageContent);
  virtual void processVibratoAmpChangeMsg(uint8_t messageContent);
  virtual void processSetResyncTimeMsg(uint16_t messageContent);

 protected:
  bool hasStarted_, isFirstAfterStart_, isFirstAfterSongStart_, isFirstAfterStop_, isBpmChangePending_;
  bool isLimbActive_[NB_HIT_JOINTS], isActivationPending_[NB_HIT_JOINTS];
  float pendingBpm_;
  uint8_t songIdx_;

 private:
  void sendRobotToRest();
  void checkPendingMessages();
  void updateResyncCompensationTime(unsigned long currTime);

  PercuRobot<NB_HIT_JOINTS, NB_POS_JOINTS> *robot_;
  PercuSong<NB_HIT_JOINTS, BITS_FOR_POS> *song_;

  unsigned long timeNextHitInstruction_[NB_HIT_JOINTS], timeNextPosInstruction_[NB_POS_JOINTS];
  byte nextInstruction_[NB_HIT_JOINTS];
  bool hasToMove_[NB_POS_JOINTS];
  byte nextPos_[NB_POS_JOINTS];

  float bpm_;
  unsigned int timePerSemiquaver_;
  unsigned long timeNextSemiquaver_;

  // Resync mechanism
  unsigned long referenceTime_;
  unsigned int resyncTime_;
  signed int resyncCompensationTime_;
  bool isResyncPending_;

  bool isSimulation_, hasOutput_;
};

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::PercuController(PercuRobot<NB_HIT_JOINTS, NB_POS_JOINTS> *robot,
                                                                             PercuSong<NB_HIT_JOINTS, BITS_FOR_POS> *song, int address,
                                                                             bool isSimulation, bool hasOutput) : RoboReceptor(address) {
  robot_ = robot;
  song_ = song;

  setTimePerSemiquaver(timesPerSemiquaver[DEFAULT_BPM_IDX]);
  hasStarted_ = false;
  isFirstAfterStart_ = false;
  isFirstAfterSongStart_ = false;
  isBpmChangePending_ = false;
  isResyncPending_ = false;

  for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
    isActivationPending_[limb] = false;
  }

  isSimulation_ = isSimulation;
  hasOutput_ = hasOutput;

  resyncCompensationTime_ = 0;
  resyncTime_ = DEFAULT_RESYNC_TIME;

  sendRobotToRest();
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::initialize(unsigned long currTime) {
  timeNextSemiquaver_ = currTime + timePerSemiquaver_;
  referenceTime_ = currTime;
  isResyncPending_ = false;
  resyncCompensationTime_ = 0;

  for (unsigned int limb = 0; limb < NB_POS_JOINTS; limb++) {
    timeNextPosInstruction_[limb] = 0;
    hasToMove_[limb] = false;
  }

  song_->goToFirstSemiquaver(hasOutput_);
  for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
    nextInstruction_[limb] = HIT;

    if (limb < NB_POS_JOINTS) {
      nextPos_[limb] = song_->getPosNextHit(limb);

      timeNextHitInstruction_[limb] = timeNextSemiquaver_ + song_->getSemiquaversToNextHit(limb) * timePerSemiquaver_ - robot_->getHitTime(limb, nextPos_[limb], song_->getVelNextHit(limb));

      robot_->rest(limb, nextPos_[limb]);
      robot_->goToPos(limb, nextPos_[limb]);
    } else {
      timeNextHitInstruction_[limb] = timeNextSemiquaver_ + song_->getSemiquaversToNextHit(limb) * timePerSemiquaver_ - robot_->getHitTime(limb, 0, song_->getVelNextHit(limb));
      robot_->rest(limb, 0, true);
    }
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
int PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::goToTime(unsigned long currTime, bool hasOutput) {
  int semiquaversEllapsed = 0;

  checkPendingMessages();

  if (hasStarted_) {
    if (isFirstAfterSongStart_) {
      song_->createPredefinedPatterns(songIdx_);
      initialize(currTime);
      isFirstAfterSongStart_ = false;
    }
    if (isFirstAfterStart_) {
      initialize(currTime);
      isFirstAfterStart_ = false;
    }
    if (isResyncPending_) {
      updateResyncCompensationTime(currTime);
      isResyncPending_ = false;
    }

    unsigned long correctedTime = currTime - resyncCompensationTime_;

    for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
      if (correctedTime >= timeNextHitInstruction_[limb]) {
        manageHitInstruction(limb, currTime);
      }
      if (isTimeToChangePos(limb, currTime)) {
        managePosInstruction(limb);
      }
    }

    while (correctedTime > timeNextSemiquaver_) {
      timeNextSemiquaver_ += timePerSemiquaver_;
      semiquaversEllapsed++;
      song_->goToNextSemiquaver();
    }
  } else {
    if (isFirstAfterStop_) {
      robot_->stop();
      isFirstAfterStop_ = false;
    }
  }
  return semiquaversEllapsed;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::manageHitInstruction(byte limb, unsigned long currTime) {
  byte currentPosition = limb < NB_POS_JOINTS ? nextPos_[limb] : 0;

  if (nextInstruction_[limb] == HIT) {
    if (isSimulation_) {
      Serial.println(robot_->getPosName(limb, currentPosition));
    } else {
      robot_->hit(limb, currentPosition, song_->getVelNextHit(limb), hasOutput_);
    }

    timeNextHitInstruction_[limb] += robot_->getHitTime(limb, currentPosition, song_->getVelNextHit(limb));
    nextInstruction_[limb] = REST;
    song_->computeNextHit(limb, hasOutput_);
  } else if (nextInstruction_[limb] == REST) {
    if (limb < NB_POS_JOINTS) {
      // We check if we need to move
      nextPos_[limb] = song_->getPosNextHit(limb);

      if (currentPosition != nextPos_[limb]) {
        hasToMove_[limb] = true;
        timeNextPosInstruction_[limb] = currTime + abs(robot_->getHitAngle(limb, currentPosition, song_->getVelNextHit(limb)) - robot_->getRestAngle(limb, nextPos_[limb])) / (POS_SECURITY_FACTOR * robot_->getServoSpeed());
        currentPosition = nextPos_[limb];
      }
      robot_->rest(limb, nextPos_[limb], hasOutput_);
    } else {
      robot_->rest(limb);
    }

    timeNextHitInstruction_[limb] = timeNextSemiquaver_ + song_->getSemiquaversToNextHit(limb) * timePerSemiquaver_ - robot_->getHitTime(limb, currentPosition, song_->getVelNextHit(limb));
    nextInstruction_[limb] = HIT;
    // We inmmediately go the the rest state of the next posiion
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::managePosInstruction(byte limb) {
  robot_->goToPos(limb, nextPos_[limb]);
  hasToMove_[limb] = false;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
bool PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::isTimeToChangePos(byte limb, unsigned long ellapsedTime) {
  return (limb < NB_POS_JOINTS && hasToMove_[limb] && ellapsedTime >= timeNextPosInstruction_[limb]);
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::setBpm(uint8_t bpm) {
  setBpm(float(bpm));
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::setBpm(float bpm) {
  bpm_ = bpm;
  unsigned long timeQuarter = int(60000.0 / bpm_);  // us per quarter note
  timePerSemiquaver_ = int(timeQuarter / 4.0);      // us per semiquaver note

  // We update the hit instructions
  for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
    if (nextInstruction_[limb] == HIT) {
      timeNextHitInstruction_[limb] = timeNextSemiquaver_ + song_->getSemiquaversToNextHit(limb) * timePerSemiquaver_ - robot_->getHitTime(limb, nextPos_[limb], song_->getVelNextHit(limb));
    }
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::setTimePerSemiquaver(unsigned long timePerSemiquaver) {
  timePerSemiquaver_ = timePerSemiquaver;  // ms per semiquaver note
  unsigned long timeQuarter = 4 * timePerSemiquaver_;
  bpm_ = int(60000.0 / timeQuarter);  // ms per quarter note
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
signed int PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::getResyncCompensationTime() {
  return resyncCompensationTime_;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processStartMsg() {
  if (!hasStarted_) {
    hasStarted_ = true;
    isFirstAfterStart_ = true;
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processStartSongMsg(uint8_t messageContent) {
  if (!hasStarted_) {
    songIdx_ = messageContent;
    hasStarted_ = true;
    isFirstAfterSongStart_ = true;
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processStopMsg() {
  if (hasStarted_) {
    hasStarted_ = false;
    isFirstAfterStop_ = true;
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processResyncMsg() {
  isResyncPending_ = true;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processBpmChangeMsg(uint8_t messageContent) {
  isBpmChangePending_ = true;
  pendingBpm_ = messageContent;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processBpmIdxChangeMsg(uint8_t messageContent) {
  isBpmChangePending_ = true;
  pendingBpm_ = bpmValues[messageContent];
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processLimbStopMsg(uint8_t messageContent) {
  if (messageContent < NB_HIT_JOINTS) {
    isLimbActive_[messageContent] = false;
    isActivationPending_[messageContent] = true;
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processLimbStartMsg(uint8_t messageContent) {
  if (messageContent < NB_HIT_JOINTS) {
    isLimbActive_[messageContent] = true;
    isActivationPending_[messageContent] = true;
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processNoteOnMsg(uint8_t messageContent) {
  if (!hasStarted_) {
    robot_->processNoteOnMessage(messageContent);
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processNoteOffMsg(uint8_t messageContent) {
  if (!hasStarted_) {
    robot_->processNoteOffMessage(messageContent);
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processVibratoAmpChangeMsg(uint8_t messageContent) {
  robot_->processVibratoAmpChangeMessage(messageContent);
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::processSetResyncTimeMsg(uint16_t messageContent) {
  resyncTime_ = messageContent;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::sendRobotToRest() {
  for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
    robot_->rest(limb);
    robot_->goToPos(limb, 0);
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::checkPendingMessages() {
  if (isBpmChangePending_) {
    setBpm(pendingBpm_);
    isBpmChangePending_ = false;
  }
  for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
    if (isActivationPending_[limb]) {
      if (isLimbActive_[limb]) {
        robot_->activateLimb(limb);
      } else {
        robot_->deactivateLimb(limb);
      }
      isActivationPending_[limb] = false;
    }
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::updateResyncCompensationTime(unsigned long currTime) {
  resyncCompensationTime_ = (currTime - referenceTime_) - resyncTime_;
  referenceTime_ += resyncTime_;
}
#endif
