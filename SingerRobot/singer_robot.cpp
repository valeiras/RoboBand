#include "singer_robot.h"

SingerRobot::SingerRobot(byte vibratoPins[NB_SINGERS]){
  // We initialize the default parameters
  byte defaultClosedPositions[NB_SINGERS] = { DEFAULT_CLOSED_POS, DEFAULT_CLOSED_POS, DEFAULT_CLOSED_POS };
  byte defaultOpenPositions[NB_SINGERS] = { DEFAULT_OPEN_POS, DEFAULT_OPEN_POS, DEFAULT_OPEN_POS };

  setVibratoParams(defaultClosedPositions, defaultOpenPositions, DEFAULT_VIBRATO_AMP);

  setServoSpeed(_wServoSg);
  hasVibrato_ = false;
  stop();

  for (unsigned int ii = 0; ii < NB_SINGERS; ii++) {
    vibratoServos_[ii].attach(vibratoPins[ii]);

    noteOn_[ii] = 0;
    noteOnPending_[ii] = false;
    noteOffPending_[ii] = false;
    currNoteIdx_[ii] = 0;
  }

  setFrequencies();

  currSinger_ = 0;
}

unsigned int SingerRobot::getFrequency(byte singerIdx) {
  if (singerIdx < NB_SINGERS) {
    return frequencies_[currNoteIdx_[singerIdx]];
  } else {
    return 0;
  }
}

void SingerRobot::hit(byte singerIdx, byte pos, byte noteIdx, bool hasOutput = 0) {
  if (isNoteOn(currSinger_)) {
    makeNoteOff(currSinger_);
  }

  if (noteIdx != OFFS) {
    currSinger_ = ++currSinger_ % NB_SINGERS;
    makeNoteOn(currSinger_, min(noteIdx, NB_NOTES_SG - 1), hasOutput);
  }
}

void SingerRobot::rest(byte singerIdx, byte pos = 0, bool hasOutput = 0) {
}

void SingerRobot::processNoteOnMessage(byte noteIdx) {
  makeNoteOn(currSinger_, noteIdx - FIRST_AVAILABLE_NOTE_IDX + NOTE_ON);
  currSinger_ = ++currSinger_ % NB_SINGERS;
}

void SingerRobot::processNoteOffMessage(byte noteIdx) {
  for (unsigned int ii = 0; ii < NB_SINGERS; ii++) {
    if (noteOn_[ii] && currNoteIdx_[ii] == noteIdx - FIRST_AVAILABLE_NOTE_IDX + NOTE_ON) {
      makeNoteOff(ii);
      break;
    }
  }
}

void SingerRobot::processVibratoAmpChangeMessage(byte vibratoAmp) {
  vibratoAmplitude_ = vibratoAmp;
  hasVibrato_ = vibratoAmp != 0;
}

void SingerRobot::stop() {
  for (unsigned int ii = 0; ii < NB_SINGERS; ii++) {
    vibratoServos_[ii].write(closedPosition_[ii]);
    makeNoteOff(ii);
  }
}

void SingerRobot::makeNoteOn(byte singerIdx, byte noteIdx, bool hasOutput = 0) {
  noteOnPending_[singerIdx] = true;
  noteOn_[singerIdx] = true;
  currNoteIdx_[singerIdx] = noteIdx;
}

void SingerRobot::makeNoteOff(byte singerIdx) {
  noteOffPending_[singerIdx] = true;
  noteOn_[singerIdx] = false;
}

bool SingerRobot::isNoteOn(byte singerIdx) {
  return noteOn_[singerIdx];
}

bool SingerRobot::isNoteOnPending(byte singerIdx) {
  return noteOnPending_[singerIdx];
}

bool SingerRobot::isNoteOffPending(byte singerIdx) {
  return noteOffPending_[singerIdx];
}

bool SingerRobot::unsetNoteOnPending(byte singerIdx) {
  noteOnPending_[singerIdx] = false;
}

bool SingerRobot::unsetNoteOffPending(byte singerIdx) {
  noteOffPending_[singerIdx] = false;
}

void SingerRobot::startVibrato(byte singerIdx, unsigned long currTime, bool hasOutput = 0) {
  vibratoDirection_[singerIdx] = 1;
  vibratoServos_[singerIdx].write(openPosition_[singerIdx]);
  nextVibratoInstructionTime_[singerIdx] = currTime + abs((openPosition_[singerIdx] + vibratoAmplitude_ - closedPosition_[singerIdx]) / wServo_);
}

void SingerRobot::stopVibrato(byte singerIdx, bool hasOutput = 0) {
  vibratoServos_[singerIdx].write(closedPosition_[singerIdx]);
}

void SingerRobot::checkVibrato(byte singerIdx, unsigned long currTime, bool hasOutput = 0) {
  if (hasVibrato_) {
    if (noteOn_[singerIdx] && currTime > nextVibratoInstructionTime_[singerIdx]) {
      goToNextVibratoPosition(singerIdx);
    }
  }
}

void SingerRobot::setVibrato(bool hasVibrato) {
  hasVibrato_ = hasVibrato;
}

void SingerRobot::goToNextVibratoPosition(byte singerIdx) {
  //We switch the vibrato direction between 0 and 1
  vibratoDirection_[singerIdx] = vibratoDirection_[singerIdx] == 1 ? 0 : 1;
  vibratoServos_[singerIdx].write(openPosition_[singerIdx] + vibratoDirection_[singerIdx] * vibratoAmplitude_);
  nextVibratoInstructionTime_[singerIdx] += 2 * vibratoAmplitude_ / wServo_;
}

void SingerRobot::setVibratoParams(byte closedPos[NB_SINGERS], byte openPos[NB_SINGERS], byte vibratoAmp) {
  for (unsigned int ii = 0; ii < NB_SINGERS; ii++) {
    closedPosition_[ii] = closedPos[ii];
    openPosition_[ii] = openPos[ii];
  }
  vibratoAmplitude_ = vibratoAmp;
}

void SingerRobot::setLimbParams() {
}

void SingerRobot::setFrequencies() {
  frequencies_.push_back(NOTE_OFF);
  frequencies_.push_back(FREQ_C02);
  frequencies_.push_back(FREQ_CS2);
  frequencies_.push_back(FREQ_D02);
  frequencies_.push_back(FREQ_DS2);
  frequencies_.push_back(FREQ_E02);
  frequencies_.push_back(FREQ_F02);
  frequencies_.push_back(FREQ_FS2);
  frequencies_.push_back(FREQ_G02);
  frequencies_.push_back(FREQ_GS2);
  frequencies_.push_back(FREQ_A02);
  frequencies_.push_back(FREQ_AS2);
  frequencies_.push_back(FREQ_B02);
  frequencies_.push_back(FREQ_C03);
  frequencies_.push_back(FREQ_CS3);
  frequencies_.push_back(FREQ_D03);
  frequencies_.push_back(FREQ_DS3);
  frequencies_.push_back(FREQ_E03);
  frequencies_.push_back(FREQ_F03);
  frequencies_.push_back(FREQ_FS3);
  frequencies_.push_back(FREQ_G03);
  frequencies_.push_back(FREQ_GS3);
  frequencies_.push_back(FREQ_A03);
  frequencies_.push_back(FREQ_AS3);
  frequencies_.push_back(FREQ_B03);
  frequencies_.push_back(FREQ_C04);
  frequencies_.push_back(FREQ_CS4);
  frequencies_.push_back(FREQ_D04);
  frequencies_.push_back(FREQ_DS4);
  frequencies_.push_back(FREQ_E04);
  frequencies_.push_back(FREQ_F04);
  frequencies_.push_back(FREQ_FS4);
  frequencies_.push_back(FREQ_G04);
  frequencies_.push_back(FREQ_GS4);
  frequencies_.push_back(FREQ_A04);
  frequencies_.push_back(FREQ_AS4);
  frequencies_.push_back(FREQ_B04);
}