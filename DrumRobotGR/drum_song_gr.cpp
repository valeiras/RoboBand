#include "drum_song_gr.h"

DrumSongGR::DrumSongGR()
  : PercuSong<NB_LIMBS_DR, BITS_FOR_POS_DR>::PercuSong() {
  nbOfPositions_[RIGHT_ARM_DR] = NB_POS_RA_DR;
  nbOfPositions_[LEFT_ARM_DR] = NB_POS_LA_DR;
  nbOfPositions_[RIGHT_LEG_DR] = NB_POS_RL_DR;
}

void DrumSongGR::createPredefinedPatterns(Songs songName, bool hasOutput = false) {
  switch (songName) {
    case FRERE_JACQUES:
      {
        // -------------------------------------------- FRERE JACQUES    -----------------------------------------------------
        nbPatterns_ = 3;
        nbMeasures_ = 9;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId0 = 0;
        byte patternId1 = 1;
        byte patternId2 = 2;

        // Right leg
        // -----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId0, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId0, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_LEG_DR, patternId1, BDRU, REST, REST, REST, REST, REST, REST, REST, BDRU, REST, BDRU, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId1, V012, V000, V000, V000, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_LEG_DR, patternId2, BDRU, REST, REST, REST, REST, REST, REST, REST, BDRU, REST, BDRU, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId2, V012, V000, V000, V000, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);


        // Left arm
        // ---------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId0, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId0, V012, V000, V000, V000, V010, V000, V000, V000, V012, V000, V000, V000, V010, V000, V000, V000, hasOutput);

        setHitPattern(LEFT_ARM_DR, patternId1, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId1, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V012, V000, V007, V000, hasOutput);

        setHitPattern(LEFT_ARM_DR, patternId2, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, REST, SNLT, REST, SNLT, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId2, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V000, V006, V000, V005, hasOutput);

        // Right arm
        // ----------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId0, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId0, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_ARM_DR, patternId1, CRSH, REST, REST, REST, SNRG, REST, REST, SNRG, REST, REST, REST, REST, SNRG, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId1, V014, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V012, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_ARM_DR, patternId2, REST, REST, REST, REST, SNRG, REST, REST, SNRG, REST, REST, REST, REST, SNRG, REST, SNRG, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId2, V000, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V014, V000, V012, V000, hasOutput);

        // Right leg
        // ----------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId0, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId0, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_LEG_DR, patternId1, BDRU, REST, REST, REST, REST, REST, REST, REST, BDRU, REST, BDRU, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId1, V012, V000, V000, V000, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_LEG_DR, patternId2, BDRU, REST, REST, REST, REST, REST, REST, REST, BDRU, REST, BDRU, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId2, V012, V000, V000, V000, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);


        // Left arm
        // ---------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId0, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId0, V012, V000, V000, V000, V010, V000, V000, V000, V012, V000, V000, V000, V010, V000, V000, V000, hasOutput);

        setHitPattern(LEFT_ARM_DR, patternId1, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId1, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V012, V000, V007, V000, hasOutput);

        setHitPattern(LEFT_ARM_DR, patternId2, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, REST, SNLT, REST, SNLT, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId2, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V000, V006, V000, V005, hasOutput);

        // Right arm
        // ----------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId0, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, STCK, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId0, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_ARM_DR, patternId1, CRSH, REST, REST, REST, SNRG, REST, REST, SNRG, REST, REST, REST, REST, SNRG, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId1, V014, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V012, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_ARM_DR, patternId2, REST, REST, REST, REST, SNRG, REST, REST, SNRG, REST, REST, REST, REST, SNRG, REST, SNRG, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId2, V000, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V014, V000, V012, V000, hasOutput);

        byte frereJacquesPattSeq[nbMeasures_] = { 0, 1, 2, 1, 2, 1, 2, 1, 2 };

        setPatternSequence(frereJacquesPattSeq);
        break;
      }
      // -------------------------------------------- SIMPLEST RYTHM -----------------------------------------------------
    case HOUND_DOG:
      {
        nbPatterns_ = 3;
        nbMeasures_ = 6;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId0 = 0;
        byte patternId1 = 1;
        byte patternId2 = 2;

        // --------------------------------------------- Pattern 0--------------------------------------------------------------

        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId0, 1, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId0, 2, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId0, 3, hasOutput);

        setSemiquaverHit(LEFT_ARM_DR, SNLT, V014, patternId0, 2, hasOutput);
        setSemiquaverHit(LEFT_ARM_DR, SNLT, V008, patternId0, 4, hasOutput);
        setSemiquaverHit(LEFT_ARM_DR, SNLT, V006, patternId0, 6, hasOutput);
        setSemiquaverHit(LEFT_ARM_DR, SNLT, V009, patternId0, 8, hasOutput);

        setSemiquaverHit(RIGHT_ARM_DR, SNRG, V012, patternId0, 1, hasOutput);
        setSemiquaverHit(RIGHT_ARM_DR, SNRG, V011, patternId0, 3, hasOutput);
        setSemiquaverHit(RIGHT_ARM_DR, CRSH, V013, patternId0, 5, hasOutput);
        setSemiquaverHit(RIGHT_ARM_DR, CRSH, V012, patternId0, 7, hasOutput);
        setSemiquaverHit(RIGHT_ARM_DR, CRSH, V015, patternId0, 9, hasOutput);

        // --------------------------------------------- Pattern 1--------------------------------------------------------------

        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId1, 1, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId1, 3, hasOutput);

        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId1, 1, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId1, 2, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId1, 3, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V008, patternId1, 4, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V010, patternId1, 5, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V006, patternId1, 6, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId1, 7, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V009, patternId1, 8, hasOutput);

        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId1, 2, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId1, 4, hasOutput);

        // --------------------------------------------- Pattern 2--------------------------------------------------------------
        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId2, 1, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId2, 3, hasOutput);
        setSemiquaverHit(RIGHT_LEG_DR, BDRU, V014, patternId2, 11, hasOutput);

        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId2, 1, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId2, 2, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId2, 3, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V008, patternId2, 4, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V010, patternId2, 5, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V006, patternId2, 6, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId2, 7, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V009, patternId2, 8, hasOutput);

        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId2, 1, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId2, 2, hasOutput);
        setSemiquaverHit(RIGHT_ARM_DR, SNRG, V014, patternId2, 8, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, CRSH, V015, patternId2, 4, hasOutput);

        byte houndDogPattSeq[nbMeasures_] = { 0, 1, 2, 1, 2, 1 };
        setPatternSequence(houndDogPattSeq);

        break;
      }
      // -------------------------------------------- RUDY RYTHM -----------------------------------------------------
    case RUDY:
      {
        nbPatterns_ = 3;
        nbMeasures_ = 8;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId0 = 0;
        byte patternId1 = 1;
        byte patternId2 = 2;

        // --------------------------------------------- Pattern 0--------------------------------------------------------------
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId0, 4, hasOutput);

        setQuarterHit(RIGHT_ARM_DR, CRSH, V013, patternId0, 1, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, CRSH, V012, patternId0, 2, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, CRSH, V015, patternId0, 3, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, CRSH, V015, patternId0, 4, hasOutput);

        // --------------------------------------------- Pattern 1--------------------------------------------------------------

        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId1, 1, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId1, 3, hasOutput);

        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId1, 1, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId1, 2, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId1, 3, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V008, patternId1, 4, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V010, patternId1, 5, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V006, patternId1, 6, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId1, 7, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V009, patternId1, 8, hasOutput);

        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId1, 2, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId1, 4, hasOutput);

        // --------------------------------------------- Pattern 2--------------------------------------------------------------
        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId2, 1, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId2, 3, hasOutput);
        setSemiquaverHit(RIGHT_LEG_DR, BDRU, V014, patternId2, 11, hasOutput);

        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId2, 1, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId2, 2, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId2, 3, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V008, patternId2, 4, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V010, patternId2, 5, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V006, patternId2, 6, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId2, 7, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V009, patternId2, 8, hasOutput);

        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId2, 2, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, SNRG, V015, patternId2, 4, hasOutput);

        byte rudyPattSeq[nbMeasures_] = { 0, 1, 2, 1, 2, 1, 2, 1 };
        setPatternSequence(rudyPattSeq);

        break;
      }
      // -------------------------------------------- SMOKE -----------------------------------------------------
    case SMOKE:
      {
        nbPatterns_ = 3;
        nbMeasures_ = 22;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId0 = 0;
        byte patternId1 = 1;
        byte patternId2 = 2;

        // Right leg
        // -----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId0, BDRU, REST, REST, REST, REST, REST, REST, REST, BDRU, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId0, V012, V000, V000, V000, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_LEG_DR, patternId1, BDRU, REST, REST, REST, REST, REST, REST, REST, BDRU, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId1, V012, V000, V000, V000, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_LEG_DR, patternId2, BDRU, REST, REST, REST, BDRU, REST, REST, REST, BDRU, REST, REST, REST, BDRU, REST, BDRU, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId2, V012, V000, V000, V000, V012, V000, V000, V000, V008, V000, V015, V000, V006, V000, V004, V000, hasOutput);


        // Left arm
        // ----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId0, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, HHAT, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId0, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V012, V000, V007, V000, hasOutput);

        setHitPattern(LEFT_ARM_DR, patternId1, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId1, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V012, V006, V008, V005, hasOutput);

        setHitPattern(LEFT_ARM_DR, patternId2, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId2, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V012, V006, V008, V005, hasOutput);

        // Right arm
        // ---------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId0, REST, REST, REST, REST, SNRG, REST, REST, REST, REST, REST, REST, REST, SNRG, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId0, V014, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V012, V000, V000, V000, hasOutput);

        setHitPattern(RIGHT_ARM_DR, patternId1, REST, REST, REST, REST, SNRG, REST, REST, REST, REST, REST, REST, REST, SNRG, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId1, V000, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V014, V000, V012, V000, hasOutput);

        setHitPattern(RIGHT_ARM_DR, patternId2, CRSH, REST, REST, REST, CRSH, REST, REST, REST, CRSH, REST, REST, REST, CRSH, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId2, V012, V000, V000, V000, V012, V000, V000, V008, V012, V000, V000, V000, V014, V000, V012, V000, hasOutput);

        byte smokePattSeq[nbMeasures_] = { 0, 0, 0, 0, 0, 0, 0, 0,
                                           1, 1, 1, 1, 1, 1, 1, 1,
                                           2, 2, 1, 1, 2, 2 };
        setPatternSequence(smokePattSeq);

        break;
      }

      // -------------------------------------------- SMELLS LIKE -----------------------------------------------------
    case SMELLS_LIKE:
      {
        nbPatterns_ = 4;
        nbMeasures_ = 20;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId0 = 0;
        byte patternId1 = 1;
        byte patternId2 = 2;
        byte patternId3 = 3;

        // Pattern 0: empty
        // -----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId0, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        // ----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId0, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        // ----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId0, CRSH, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId0, V014, V000, V000, V000, V012, V000, V000, V008, V000, V000, V000, V000, V000, V000, V000, V000, hasOutput);


        // Pattern 1: intro
        // -----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId1, REST, BDRU, REST, BDRU, REST, BDRU, REST, BDRU, REST, BDRU, REST, BDRU, REST, BDRU, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId1, V000, V012, V000, V012, V000, V000, V000, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        // ----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId1, SNLT, REST, REST, REST, SNLT, REST, REST, REST, SNLT, REST, REST, REST, SNLT, REST, REST, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId1, V012, V000, V008, V000, V012, V000, V007, V000, V014, V000, V007, V000, V012, V006, V008, V005, hasOutput);

        // ---------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId1, SNRG, REST, REST, REST, SNRG, REST, REST, REST, SNRG, REST, REST, REST, SNRG, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId1, V015, V000, V000, V000, V012, V000, V000, V008, V015, V000, V000, V000, V014, V000, V012, V000, hasOutput);


        // Pattern 2: chorus
        // -----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId2, BDRU, REST, REST, BDRU, REST, REST, BDRU, REST, BDRU, REST, BDRU, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId2, V012, V000, V000, V012, V000, V000, V012, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        // ----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId2, REST, REST, REST, REST, SNLT, REST, REST, SNLT, REST, SNLT, REST, REST, SNLT, REST, REST, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId2, V012, V000, V008, V000, V012, V000, V007, V012, V014, V012, V007, V000, V012, V000, V007, V000, hasOutput);

        // ---------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId2, CRSH, REST, REST, REST, CRSH, REST, REST, REST, CRSH, REST, REST, REST, CRSH, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId2, V012, V000, V000, V000, V012, V000, V000, V008, V012, V000, V000, V000, V014, V000, V012, V000, hasOutput);

        // Pattern 3: verse
        // -----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_LEG_DR, patternId3, BDRU, REST, BDRU, REST, REST, REST, REST, REST, BDRU, REST, BDRU, REST, REST, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_LEG_DR, patternId3, V012, V000, V000, V012, V000, V000, V012, V000, V008, V000, V015, V000, V000, V000, V000, V000, hasOutput);

        // ----------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(LEFT_ARM_DR, patternId3, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, HHAT, REST, hasOutput);
        setVelPattern(LEFT_ARM_DR, patternId3, V012, V000, V008, V000, V012, V000, V007, V000, V014, V012, V007, V000, V012, V000, V007, V000, hasOutput);

        // ---------------------------------------------0001--------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(RIGHT_ARM_DR, patternId3, CRSH, REST, REST, REST, SNRG, REST, REST, REST, REST, REST, REST, REST, SNRG, REST, REST, REST, hasOutput);
        setVelPattern(RIGHT_ARM_DR, patternId3, V012, V000, V000, V000, V012, V000, V000, V008, V012, V000, V000, V000, V014, V000, V012, V000, hasOutput);

        byte smellsLikePattSeq[nbMeasures_] = { 0, 0, 0, 1, 2, 2, 2, 2,
                                                3, 3, 3, 3, 3, 3, 3, 3,
                                                2, 2, 2, 2 };
        setPatternSequence(smellsLikePattSeq);

        break;
      }

      // -------------------------------------------- BASIC RYTHM -----------------------------------------------------
    case BASIC_RYTHM:
    default:
      {
        nbPatterns_ = 1;
        nbMeasures_ = 1;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId = 0;

        setQuarterHit(RIGHT_LEG_DR, BDRU, V012, patternId, 1, hasOutput);
        setQuarterHit(RIGHT_LEG_DR, BDRU, V008, patternId, 3, hasOutput);
        setQuaverHit(RIGHT_LEG_DR, BDRU, V014, patternId, 6, hasOutput);

        setQuaverHit(LEFT_ARM_DR, HHAT, V014, patternId, 1, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V007, patternId, 2, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId, 3, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V008, patternId, 4, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V010, patternId, 5, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V006, patternId, 6, hasOutput);
        setQuaverHit(LEFT_ARM_DR, HHAT, V012, patternId, 7, hasOutput);
        setQuaverHit(LEFT_ARM_DR, SNLT, V009, patternId, 8, hasOutput);

        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId, 1, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, SNRG, V014, patternId, 2, hasOutput);
        setQuarterHit(RIGHT_ARM_DR, CRSH, V015, patternId, 4, hasOutput);

        byte basicPattSeq[nbMeasures_] = { 0 };
        setPatternSequence(basicPattSeq);

        break;
      }
  }
}