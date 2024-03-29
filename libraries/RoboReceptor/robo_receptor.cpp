#include "robo_receptor.h"

RoboReceptor::RoboReceptor() {
}

RoboReceptor::RoboReceptor(int address) {
  Wire.begin(address);
  Wire.onReceive(receiveMessage);
}

RoboReceptor::setReceptor() {
  receptor = this;
}

uint16_t arrayToUint16(uint8_t* arr) {
  return arr[0] | (arr[1] << BYTES_PER_UINT8 * BITS_PER_BYTE);
}

void receiveMessage(int howMany) {
  MessageType msgType = Wire.read();

  uint8_t msgContent8;
  uint16_t msgContent16;

  if (howMany == 2) {
    msgContent8 = Wire.read();
  } else if (howMany == 3) {
    uint8_t dataArr[2];
    dataArr[0] = Wire.read();
    dataArr[1] = Wire.read();
    msgContent16 = arrayToUint16(dataArr);
  }

  switch (msgType) {
    case START:
      receptor->processStartMsg();
      break;
    case STOP:
      receptor->processStopMsg();
      break;
    case RESYNC:
      receptor->processResyncMsg();
      break;
    case START_SONG:
      receptor->processStartSongMsg(msgContent8);
      break;
    case BPM_CHANGE:
      receptor->processBpmChangeMsg(msgContent8);
      break;
    case BPM_IDX_CHANGE:
      receptor->processBpmIdxChangeMsg(msgContent8);
      break;
    case MTX_MODE_CHANGE:
      receptor->processMtxModeChangeMsg(msgContent8);
      break;
    case SPL_MODE_CHANGE:
      receptor->processSplModeChangeMsg(msgContent8);
      break;
    case MTX_BLINK_CHANGE:
      receptor->processMtxBlinkChangeMsg(msgContent8);
      break;
    case SPL_BLINK_CHANGE:
      receptor->processSplBlinkChangeMsg(msgContent8);
      break;
    case BRIGHTNESS_CHANGE:
      receptor->processBrightnessChangeMsg(msgContent8);
      break;
    case LIMB_STOP:
      receptor->processLimbStopMsg(msgContent8);
      break;
    case LIMB_START:
      receptor->processLimbStartMsg(msgContent8);
      break;
    case NOTE_ON:
      receptor->processNoteOnMsg(msgContent8);
      break;
    case NOTE_OFF:
      receptor->processNoteOffMsg(msgContent8);
      break;
    case VIBRATO_AMP_CHANGE:
      receptor->processVibratoAmpChangeMsg(msgContent8);
      break;
    case SET_RESYNC_TIME:
      receptor->processSetResyncTimeMsg(msgContent16);
      break;
  }
}

// We provide empty implementations of some of these functions, as not all of the robots need them
void RoboReceptor::processBrightnessChangeMsg(uint8_t messageContent) {}
void RoboReceptor::processLimbStopMsg(uint8_t messageContent) {}
void RoboReceptor::processLimbStartMsg(uint8_t messageContent) {}
void RoboReceptor::processNoteOnMsg(uint8_t messageContent) {}
void RoboReceptor::processNoteOffMsg(uint8_t messageContent) {}
void RoboReceptor::processVibratoAmpChangeMsg(uint8_t messageContent) {}
void RoboReceptor::processMtxModeChangeMsg(uint8_t messageContent) {}
void RoboReceptor::processSplModeChangeMsg(uint8_t messageContent) {}
void RoboReceptor::processMtxBlinkChangeMsg(uint8_t messageContent) {}
void RoboReceptor::processSplBlinkChangeMsg(uint8_t messageContent) {}
