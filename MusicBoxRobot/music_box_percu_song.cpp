#include "music_box_percu_song.h"

MusicBoxPercuSong::MusicBoxPercuSong()
  : PercuSong<NB_HIT_JOINTS_MB, BITS_FOR_POS_MB>::PercuSong() {
}

void MusicBoxPercuSong::createPredefinedPatterns(byte songName, bool printOutput = false) {
  switch (songName) {
    // -------------------------------------------- SIMPLEST -----------------------------------------------------
    case SIMPLEST_RYTHM:
    default:
      {
        nbPatterns_ = 2;
        nbMeasures_ = 8;
        initializeBlankPatterns(nbPatterns_, nbMeasures_);

        byte patternId0 = 0;
        byte patternId1 = 1;

        // Foot
        // -----------------------------------------------0001-------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(FOOT_MB, patternId0, HITP, REST, REST, REST, HITP, REST, REST, REST, HITP, REST, REST, REST, HITP, REST, REST, REST, printOutput);
        setVelPattern(FOOT_MB, patternId0, V015, V000, V000, V000, V006, V000, V000, V000, V008, V000, V000, V000, V015, V000, V000, V000, printOutput);

        setHitPattern(FOOT_MB, patternId1, HITP, REST, REST, REST, HITP, REST, REST, REST, HITP, REST, REST, REST, HITP, REST, REST, REST, printOutput);
        setVelPattern(FOOT_MB, patternId1, V015, V000, V000, V000, V006, V000, V000, V000, V008, V000, V000, V000, V015, V000, V000, V000, printOutput);

        // Head
        // -----------------------------------------------0001-------////--------0002--------////--------0003--------////--------0004--------////------
        setHitPattern(HEAD_MB, patternId0, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, printOutput);
        setVelPattern(HEAD_MB, patternId0, V015, V000, V000, V000, V006, V000, V000, V000, V008, V000, V000, V000, V015, V000, V000, V000, printOutput);

        setHitPattern(HEAD_MB, patternId1, HITP, REST, REST, REST, HITP, REST, REST, REST, HITP, REST, REST, REST, HITP, REST, REST, REST, printOutput);
        setVelPattern(HEAD_MB, patternId1, V015, V000, V000, V000, V006, V000, V000, V000, V008, V000, V000, V000, V015, V000, V000, V000, printOutput);

        byte simplestPattSeq[nbMeasures_] = { 0, 1, 1, 1, 1, 1, 1, 1 };
        setPatternSequence(simplestPattSeq);

        break;
      }
  }
}