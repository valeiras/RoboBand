#include <Wire.h>
#include <MIDI.h>
#include <robo_communication.h>

#include "midi_notes.h"

#define CC_MIN 0
#define CC_MAX 127
#define MAX_BRIGHTNESS 64

#define MIN_VIBRATO_AMP 0
#define MAX_VIBRATO_AMP 20
#define LED_PIN 13

#define MIN_BPM_CHANGE 5
#define MIN_BPM_IDX 15

#define LEFT_LIMB 0
#define RIGHT_LIMB 1
#define MB_LIMB 0

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

uint8_t bpmIdx = DEFAULT_BPM_IDX;
uint8_t bpm = 100;
uint8_t mtxBlinkInterval = QUARTER_INTERVAL;
uint8_t splBlinkInterval = QUARTER_INTERVAL;
uint8_t vibratoAmp = 0;
uint8_t currSongIdx = 0;

unsigned long lastBpmTime = 0, lastBrightnessTime = 0, lastMtxBlinkTime = 0, lastSplBlinkTime = 0, lastVibratoAmpTime = 0;
byte lastBpmControllerVal, lastBrightnessControllerVal, lastMtxBlinkControllerVal, lastSplBlinkControllerVal, lastVibratoAmpControllerVal;
bool isBpmPending = false, isBrightnessPending = false, isMtxBlinkPending = false, isSplBlinkPending = false, isVibratoAmpPending = false;

// Resync mechanism
bool hasStarted = false, isStartPending = false;
unsigned long startTime;
uint16_t resyncTime = DEFAULT_RESYNC_TIME;

unsigned int minTimeInterval = 100;

short minBpm = 60;
short maxBpm = 150;

bool isRobotPresent[NB_ROBOTS];

void setup() {
  isRobotPresent[DRUM_ROBOT] = true;
  isRobotPresent[GLOCKEN_ROBOT] = true;
  isRobotPresent[MUSIC_BOX_ROBOT] = false;
  isRobotPresent[LIGHTING_ROBOT] = false;
  isRobotPresent[SINGER_ROBOT] = true;

  MIDI.begin(MIDI_CHANNEL_OMNI);  // Initialize the Midi Library.
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleCCMessage);

  Wire.begin();
  notifyRobots(STOP);
  //delay(10000);
  notifyRobots(SET_RESYNC_TIME, resyncTime);

  pinMode(LED_PIN, OUTPUT);
  //notifyRobots(START_SONG, _songs[0]);
  // isStartPending = true;
}

void loop() {
  MIDI.read();
  unsigned long currTime = millis();
  checkStartPending(currTime);
  checkResync(currTime);
  checkCCPending(currTime);
}

void notifyRobots(uint8_t messageType) {
  for (unsigned int ii = 0; ii < NB_ROBOTS; ii++) {
    sendMessage(ii, messageType);
  }
}

void notifyRobots(uint8_t messageType, uint8_t messageContent) {
  for (unsigned int ii = 0; ii < NB_ROBOTS; ii++) {
    sendMessage(ii, messageType, messageContent);
  }
}

void notifyRobots(uint8_t messageType, uint16_t messageContent) {
  for (unsigned int ii = 0; ii < NB_ROBOTS; ii++) {
    sendMessage(ii, messageType, messageContent);
  }
}

void sendMessage(uint8_t robot, uint8_t messageType) {
  if (isRobotPresent[robot]) {
    Wire.beginTransmission(_robotAddress[robot]);
    Wire.write(messageType);
    Wire.endTransmission();
  }
}

void sendMessage(uint8_t robot, uint8_t messageType, uint8_t messageContent) {
  if (isRobotPresent[robot]) {
    Wire.beginTransmission(_robotAddress[robot]);
    Wire.write(messageType);
    Wire.write(messageContent);
    Wire.endTransmission();
  }
}

void sendMessage(uint8_t robot, uint8_t messageType, uint16_t messageContent) {
  if (isRobotPresent[robot]) {
    uint8_t messageArray[BYTES_PER_UINT16];
    uint16ToArray(messageContent, messageArray);

    Wire.beginTransmission(_robotAddress[robot]);
    Wire.write(messageType);
    Wire.write(messageArray, BYTES_PER_UINT16);
    Wire.endTransmission();
  }
}

void uint16ToArray(uint16_t inputNumber, uint8_t* arr) {
  arr[0] = inputNumber & MASK_BYTE0;
  arr[1] = (inputNumber & MASK_BYTE1) >> BYTES_PER_UINT8 * BITS_PER_BYTE;
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH);
  switch (pitch) {
    case START_KEY_0:
      isStartPending = !hasStarted;
      currSongIdx = 0;
      break;
    case START_KEY_1:
      isStartPending = !hasStarted;
      currSongIdx = 1 % NB_SONGS; // We make an additional check, in case not enough song have been defined
      break;
    case START_KEY_2:
      isStartPending = !hasStarted;
      currSongIdx = 2 % NB_SONGS;
      break;
    case STOP_KEY:
      notifyRobots(STOP);
      hasStarted = false;
      break;
    case MTX_BLINK_KEY:
      sendMessage(LIGHTING_ROBOT, MTX_MODE_CHANGE, uint8_t(MTX_BLINK_MODE));
      break;
    case MTX_NAME_KEY:
      sendMessage(LIGHTING_ROBOT, MTX_MODE_CHANGE, uint8_t(MTX_NAME_MODE));
      break;
    case MTX_LOGO_KEY:
      sendMessage(LIGHTING_ROBOT, MTX_MODE_CHANGE, uint8_t(MTX_LOGO_MODE));
      break;
    case MTX_RCT_KEY:
      sendMessage(LIGHTING_ROBOT, MTX_MODE_CHANGE, uint8_t(MTX_RCT_MODE));
      break;
    case MTX_BARS_KEY:
      sendMessage(LIGHTING_ROBOT, MTX_MODE_CHANGE, uint8_t(MTX_BARS_MODE));
      break;
    case SPL_TOP_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_TOP_ON));
      break;
    case SPL_BOTTOM_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_BOTTOM_ON));
      break;
    case SPL_BLINK_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_BLINK_ON));
      break;
    case SPL_SEQ_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_SEQ_ON));
      break;
    case MB_STOP_KEY:
      //sendMessage(MUSIC_BOX_ADDRESS, LIMB_STOP, uint8_t(MB_LIMB));
      sendMessage(DRUM_ROBOT, LIMB_STOP, uint8_t(LEFT_LIMB));
      sendMessage(DRUM_ROBOT, LIMB_STOP, uint8_t(RIGHT_LIMB));
      sendMessage(GLOCKEN_ROBOT, LIMB_STOP, uint8_t(LEFT_LIMB));
      sendMessage(GLOCKEN_ROBOT, LIMB_STOP, uint8_t(RIGHT_LIMB));
      break;
    case DRUM_LA_STOP_KEY:
      sendMessage(DRUM_ROBOT, LIMB_STOP, uint8_t(LEFT_LIMB));
      break;
    case DRUM_RA_STOP_KEY:
      sendMessage(DRUM_ROBOT, LIMB_STOP, uint8_t(RIGHT_LIMB));
      break;
    case GLOCK_LA_STOP_KEY:
      sendMessage(GLOCKEN_ROBOT, LIMB_STOP, uint8_t(LEFT_LIMB));
      break;
    case GLOCK_RA_STOP_KEY:
      sendMessage(GLOCKEN_ROBOT, LIMB_STOP, uint8_t(RIGHT_LIMB));
      break;
    case C06 ... C08:
      sendMessage(SINGER_ROBOT, NOTE_ON, uint8_t(pitch));
      break;
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  digitalWrite(LED_BUILTIN, LOW);
  switch (pitch) {
    case MTX_BLINK_KEY:
    case MTX_NAME_KEY:
    case MTX_LOGO_KEY:
    case MTX_RCT_KEY:
    case MTX_BARS_KEY:
      //sendMessage(LIGHTING_ROBOT, MTX_MODE_CHANGE, uint8_t(MTX_CONSTANT_MODE));
      break;
    case SPL_TOP_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_TOP_OFF));
      break;
    case SPL_BOTTOM_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_BOTTOM_OFF));
      break;
    case SPL_BLINK_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_BLINK_OFF));
      break;
    case SPL_SEQ_KEY:
      sendMessage(LIGHTING_ROBOT, SPL_MODE_CHANGE, uint8_t(SPL_SEQ_OFF));
      break;
    case MB_STOP_KEY:
      sendMessage(DRUM_ROBOT, LIMB_START, uint8_t(LEFT_LIMB));
      sendMessage(DRUM_ROBOT, LIMB_START, uint8_t(RIGHT_LIMB));
      sendMessage(GLOCKEN_ROBOT, LIMB_START, uint8_t(LEFT_LIMB));
      sendMessage(GLOCKEN_ROBOT, LIMB_START, uint8_t(RIGHT_LIMB));
      break;
    case DRUM_LA_STOP_KEY:
      sendMessage(DRUM_ROBOT, LIMB_START, uint8_t(LEFT_LIMB));
      break;
    case DRUM_RA_STOP_KEY:
      sendMessage(DRUM_ROBOT, LIMB_START, uint8_t(RIGHT_LIMB));
      break;
    case GLOCK_LA_STOP_KEY:
      sendMessage(GLOCKEN_ROBOT, LIMB_START, uint8_t(LEFT_LIMB));
      break;
    case GLOCK_RA_STOP_KEY:
      sendMessage(GLOCKEN_ROBOT, LIMB_START, uint8_t(RIGHT_LIMB));
      break;
    case C06 ... C08:
      sendMessage(SINGER_ROBOT, NOTE_OFF, uint8_t(pitch));
      break;
  }
}

void handleCCMessage(byte channel, byte number, byte value) {
  switch (number) {
    case BPM_CONTROLLER:
      // lastBpmControllerVal = value;
      // isBpmPending = true;
      break;
    case BRIGHTNESS_CONTROLLER:
      lastBrightnessControllerVal = value;
      isBrightnessPending = true;
      break;
    case MTX_BLINK_CONTROLLER:
      lastMtxBlinkControllerVal = value;
      isMtxBlinkPending = true;
      break;
    case SPL_BLINK_CONTROLLER:
      lastSplBlinkControllerVal = value;
      isSplBlinkPending = true;
      break;
    case VIBRATO_AMP_CONTROLLER:
      lastVibratoAmpControllerVal = value;
      isVibratoAmpPending = true;
      break;
  }
}

void checkStartPending(unsigned long currTime) {
  if (isStartPending) {
    notifyRobots(BPM_CHANGE, bpm);
    notifyRobots(START_SONG, _songs[currSongIdx]);
    //currSongIdx = ++currSongIdx % NB_SONGS;
    startTime = currTime;
    hasStarted = true;
    isStartPending = false;
  }
}

void checkResync(unsigned long currTime) {
  if (hasStarted && currTime - startTime >= resyncTime) {
    startTime += resyncTime;
    notifyRobots(RESYNC);
  }
}

void checkCCPending(unsigned long currTime) {
  if (isBpmPending && currTime - lastBpmTime > minTimeInterval) {
    lastBpmTime = currTime;
    isBpmPending = false;
    int newBpmIdx = map(lastBpmControllerVal, CC_MAX, CC_MIN, MIN_BPM_IDX, NB_BPM_VALUES - 1);
    if (newBpmIdx != bpmIdx) {
      bpmIdx = newBpmIdx;
      notifyRobots(BPM_IDX_CHANGE, bpmIdx);
    }
  }

  if (isBrightnessPending && currTime - lastBrightnessTime > minTimeInterval) {
    lastBrightnessTime = currTime;
    isBrightnessPending = false;
    uint8_t newBrightness = map(lastBrightnessControllerVal, CC_MIN, CC_MAX, 0, MAX_BRIGHTNESS);
    sendMessage(LIGHTING_ROBOT, BRIGHTNESS_CHANGE, newBrightness);
  }

  if (isMtxBlinkPending && currTime - lastMtxBlinkTime > minTimeInterval) {
    lastMtxBlinkTime = currTime;
    isMtxBlinkPending = false;
    uint8_t newMtxBlinkInterval = map(lastMtxBlinkControllerVal, CC_MIN, CC_MAX, WHOLE_INTERVAL, SEMIQUAVER_INTERVAL);
    if (newMtxBlinkInterval != mtxBlinkInterval) {
      mtxBlinkInterval = newMtxBlinkInterval;
      sendMessage(LIGHTING_ROBOT, MTX_BLINK_CHANGE, newMtxBlinkInterval);
    }
  }

  if (isSplBlinkPending && currTime - lastSplBlinkTime > minTimeInterval) {
    lastSplBlinkTime = currTime;
    isSplBlinkPending = false;
    uint8_t newSplBlinkInterval = map(lastSplBlinkControllerVal, CC_MIN, CC_MAX, WHOLE_INTERVAL, SEMIQUAVER_INTERVAL);
    if (newSplBlinkInterval != splBlinkInterval) {
      splBlinkInterval = newSplBlinkInterval;
      sendMessage(LIGHTING_ROBOT, SPL_BLINK_CHANGE, newSplBlinkInterval);
    }
  }

  if (isVibratoAmpPending && currTime - lastVibratoAmpTime > minTimeInterval) {
    lastVibratoAmpTime = currTime;
    isVibratoAmpPending = false;
    uint8_t newVibratoAmp = map(lastVibratoAmpControllerVal, CC_MIN, CC_MAX, MIN_VIBRATO_AMP, MAX_VIBRATO_AMP);
    if (newVibratoAmp != vibratoAmp) {
      vibratoAmp = newVibratoAmp;
      sendMessage(SINGER_ROBOT, VIBRATO_AMP_CHANGE, newVibratoAmp);
    }
  }
}