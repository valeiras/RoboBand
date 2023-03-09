#ifndef Percu_controller_h
#define Percu_controller_h

#include <Array.h>
#include <percu_robot.h>
#include <percu_song.h>
#include <robo_receptor.h>

#include "Arduino.h"

#define POS_SECURITY_FACTOR 0.5

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
class PercuController : public RoboReceptor {
 public:
  PercuController(PercuRobot<NB_HIT_JOINTS, NB_POS_JOINTS> *robot, PercuSong<NB_HIT_JOINTS, BITS_FOR_POS> *song,
                  int address, unsigned short bpm, bool simulation, bool printOutput);

  // This method should be called once all the parameters of the robot and the song have been set
  void initializeRobot();

  void setInitialTime(unsigned long currTime);
  void goToTime(unsigned long currTime, bool printOutput = 0);

  void manageHitInstruction(byte limb, unsigned long currTime);
  void managePosInstruction(byte limb);

  bool isTimeToChangePos(byte limb, unsigned long ellapsedTime);

  void setBpm(unsigned short bpm);

  // Inherited methods from RoboReceptor
  void treatStartMsg();
  void treatResyncMsg();
  void treatBpmChangeMsg(uint8_t messageContent);
  void treatModeChangeMsg(uint8_t messageContent);
  void treatSetResyncTimeMsg(uint16_t messageContent);

 private:
  PercuRobot<NB_HIT_JOINTS, NB_POS_JOINTS> *robot_;
  PercuSong<NB_HIT_JOINTS, BITS_FOR_POS> *song_;

  unsigned long timeNextHitInstruction_[NB_HIT_JOINTS], timeNextPosInstruction_[NB_POS_JOINTS];
  byte nextInstruction_[NB_HIT_JOINTS];
  bool moveLimb_[NB_POS_JOINTS];
  byte nextPos_[NB_POS_JOINTS];

  unsigned short bpm_;
  unsigned int timePerSemiquaver_;
  unsigned long timeNextSemiquaver_, initialTime_;

  bool hasStarted_;
  bool simulation_, printOutput_;
};

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::PercuController(PercuRobot<NB_HIT_JOINTS, NB_POS_JOINTS> *robot,
                                                                             PercuSong<NB_HIT_JOINTS, BITS_FOR_POS> *song, int address,
                                                                             unsigned short bpm, bool simulation, bool printOutput) : RoboReceptor(address) {
  robot_ = robot;
  song_ = song;

  setBpm(bpm);

  simulation_ = simulation;
  printOutput_ = printOutput;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::initializeRobot() {
  for (unsigned int limb = 0; limb < NB_POS_JOINTS; limb++) {
    timeNextPosInstruction_[limb] = 0;
    moveLimb_[limb] = false;
  }

  song_->goToFirstSemiquaver(printOutput_);
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
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::setInitialTime(unsigned long initialTime) {
  timeNextSemiquaver_ = initialTime;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::goToTime(unsigned long currTime, bool printOutput) {
  if (hasStarted_) {
    unsigned long ellapsedTime = currTime - initialTime_;
    for (unsigned int limb = 0; limb < NB_HIT_JOINTS; limb++) {
      if (currTime >= timeNextHitInstruction_[limb]) {
        manageHitInstruction(limb, currTime);
      }
      if (isTimeToChangePos(limb, currTime)) {
        managePosInstruction(limb);
      }
    }

    while (currTime > timeNextSemiquaver_) {
      timeNextSemiquaver_ += timePerSemiquaver_;
      song_->goToNextSemiquaver();
    }
  } else {
    initialTime_ = currTime;
  }
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::manageHitInstruction(byte limb, unsigned long currTime) {
  byte currentPosition = limb < NB_POS_JOINTS ? nextPos_[limb] : 0;

  if (nextInstruction_[limb] == HIT) {
    if (simulation_) {
      Serial.println(robot_->getPosName(limb, currentPosition));
    }
    robot_->hit(limb, currentPosition, song_->getVelNextHit(limb), printOutput_);

    timeNextHitInstruction_[limb] += robot_->getHitTime(limb, currentPosition, song_->getVelNextHit(limb));
    nextInstruction_[limb] = REST;
    song_->computeNextHit(limb, printOutput_);
  } else if (nextInstruction_[limb] == REST) {
    if (limb < NB_POS_JOINTS) {
      // We check if we need to move
      nextPos_[limb] = song_->getPosNextHit(limb);

      if (currentPosition != nextPos_[limb]) {
        moveLimb_[limb] = true;
        timeNextPosInstruction_[limb] = currTime + abs(robot_->getHitAngle(limb, currentPosition, song_->getVelNextHit(limb)) - robot_->getRestAngle(limb, nextPos_[limb])) / (POS_SECURITY_FACTOR * robot_->getServoSpeed());
        currentPosition = nextPos_[limb];
      }
      robot_->rest(limb, nextPos_[limb], printOutput_);
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
  moveLimb_[limb] = false;
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
bool PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::isTimeToChangePos(byte limb, unsigned long ellapsedTime) {
  return (limb < NB_POS_JOINTS && moveLimb_[limb] && ellapsedTime >= timeNextPosInstruction_[limb]);
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::setBpm(unsigned short bpm) {
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
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::treatStartMsg() {
  hasStarted_ = true;
  initializeRobot();
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::treatResyncMsg() {
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::treatBpmChangeMsg(uint8_t messageContent) {
  setBpm(messageContent);
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::treatModeChangeMsg(uint8_t messageContent) {
}

template <byte NB_HIT_JOINTS, byte NB_POS_JOINTS, byte BITS_FOR_POS>
void PercuController<NB_HIT_JOINTS, NB_POS_JOINTS, BITS_FOR_POS>::treatSetResyncTimeMsg(uint16_t messageContent) {
}

#endif