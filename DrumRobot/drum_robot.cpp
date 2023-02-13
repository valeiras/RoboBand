#include "drum_robot.h"

DrumRobot::DrumRobot() {
  nbPos_[LEFT_ARM_DR] = NB_POS_LA_DR;
  nbPos_[RIGHT_ARM_DR] = NB_POS_RA_DR;
  nbPos_[RIGHT_LEG_DR] = NB_POS_RL_DR;
  nbPos_[HEAD_DR] = NB_POS_HD_DR;

  // We initialize the default parameters
  setLimbParams();
}

// We use the default values defined in robot_config.h
void DrumRobot::setLimbParams() {
  setServoSpeed(_wServoDr);

  signed char directions[NB_HIT_JOINTS_DR] = { _dirLADr, _dirRLDr, _dirRADr };

  byte anglesLeftArm[NB_POS_LA_DR][3] = { { _hitAngleSticksLeft, _restAngleSticksLeft, _posAngleSticksLeft },
                                          { _hitAngleHH, _restAngleHH, _posAngleHH },
                                          { _hitAngleSnLeft, _restAngleSnLeft, _posAngleSnLeft } };
  String namesLeftArm[NB_POS_LA_DR] = { "Sticks Left", "HH", "SN Left" };

  byte anglesRightArm[NB_POS_RA_DR][3] = { { _hitAngleSticksRight, _restAngleSticksRight, _posAngleSticksRight },
                                           { _hitAngleSnRight, _restAngleSnRight, _posAngleSnRight },
                                           { _hitAngleCrash, _restAngleCrash, _posAngleCrash } };
  String namesRightArm[NB_POS_RA_DR] = { "Sticks Right", "SN Right", "Crash" };

  byte anglesRightLeg[NB_POS_RL_DR][3] = { { _hitAngleBD, _restAngleBD, _posAngleBD } };
  String namesRightLeg[NB_POS_RL_DR] = { "BD" };

  byte anglesHead[NB_POS_HD_DR][3] = { { _hitAngleHD, _restAngleHD, _posAngleHD } };
  String namesHead[NB_POS_HD_DR] = { "HD" };

  setLimbParams(directions,
                anglesLeftArm, namesLeftArm,
                anglesRightArm, namesRightArm,
                anglesRightLeg, namesRightLeg,
                anglesHead, namesHead);
}

void DrumRobot::setLimbParams(signed char directions[NB_HIT_JOINTS_DR],
                              byte anglesLeftArm[NB_POS_LA_DR][3], String namesLeftArm[NB_POS_LA_DR],
                              byte anglesRightArm[NB_POS_RA_DR][3], String namesRightArm[NB_POS_RA_DR],
                              byte anglesRightLeg[NB_POS_RL_DR][3], String namesRightLeg[NB_POS_RL_DR],
                              byte anglesHead[NB_POS_HD_DR][3], String namesHead[NB_POS_HD_DR]) {

  for (unsigned int ii = 0; ii < NB_HIT_JOINTS_DR; ii++) {
    hitDirection_[ii] = directions[ii];
  }

  for (unsigned int ii = 0; ii < NB_POS_LA_DR; ii++) {
    posParameters_[LEFT_ARM_DR][ii] = { anglesLeftArm[ii][0], anglesLeftArm[ii][1], anglesLeftArm[ii][2], namesLeftArm[ii] };
  }

  for (unsigned int ii = 0; ii < NB_POS_RA_DR; ii++) {
    posParameters_[RIGHT_ARM_DR][ii] = { anglesRightArm[ii][0], anglesRightArm[ii][1], anglesRightArm[ii][2], namesRightArm[ii] };
  }

  for (unsigned int ii = 0; ii < NB_POS_RL_DR; ii++) {
    posParameters_[RIGHT_LEG_DR][ii] = { anglesRightLeg[ii][0], anglesRightLeg[ii][1], anglesRightLeg[ii][2], namesRightLeg[ii] };
  }

  for (unsigned int ii = 0; ii < NB_POS_HD_DR; ii++) {
    posParameters_[HEAD_DR][ii] = { anglesHead[ii][0], anglesHead[ii][1], anglesHead[ii][2], namesHead[ii] };
  }
}

byte DrumRobot::getHitAngle(byte limb, byte pos, byte velocity) {
  if (pos < nbPos_[limb]) {
    return posParameters_[limb][pos].hitAngle + hitDirection_[limb] * velocity * VEL_MULTIPLIER;
  } else {
    return 255;
  }
}

byte DrumRobot::getRestAngle(byte limb, byte pos) {
  if (pos < nbPos_[limb]) {
    return posParameters_[limb][pos].restAngle;
  } else {
    return 255;
  }
}

byte DrumRobot::getPosAngle(byte limb, byte pos) {
  if (pos < nbPos_[limb]) {
    return posParameters_[limb][pos].posAngle;
  } else {
    return 255;
  }
}

unsigned int DrumRobot::getHitTime(byte limb, byte pos, byte velocity, bool printOutput) {
  if (pos < nbPos_[limb]) {
    float result = abs(posParameters_[limb][pos].hitAngle + hitDirection_[limb] * velocity * VEL_MULTIPLIER - posParameters_[limb][pos].restAngle) / wServo_;
    if (printOutput) {
      Serial.print("Hit angle : ");
      Serial.print(posParameters_[limb][pos].hitAngle);
      Serial.print(", extra due to velocity: ");
      Serial.print(hitDirection_[limb] * velocity * VEL_MULTIPLIER);
      Serial.print(", rest angle : ");
      Serial.print(posParameters_[limb][pos].restAngle);
      Serial.print(", result:  ");
      Serial.println(result);
    }
    return round(result);
  } else {
    return 0;
  }
}

String DrumRobot::getPosName(byte limb, byte pos) {
  if (limb < NB_HIT_JOINTS_DR && pos < nbPos_[limb]) {
    return posParameters_[limb][pos].posName;
  } else {
    return "Error";
  }
}

void DrumRobot::printPosNames() {
  for (unsigned int ii = 0; ii < NB_HIT_JOINTS_DR; ii++) {
    Serial.println("");
    Serial.print("Limb ");
    Serial.print(ii);
    Serial.print(" has the following positions: ");
    for (unsigned int jj = 0; jj < nbPos_[ii]; jj++) {
      Serial.print(posParameters_[ii][jj].posName);
      Serial.print(", ");
    }
  }
}