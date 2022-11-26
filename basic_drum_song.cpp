#include "basic_drum_song.h"

BasicDrumSong::BasicDrumSong(){
}

BasicDrumSong::BasicDrumSong(unsigned short bpm): DrumSong(bpm){
  nbPatterns_ = 3;
  nbBeats_ = 10;
  createPatterns();
}

void BasicDrumSong::createPatterns(){
  hitPatternsRightLeg_[0] = B00000000;
  hitPatternsRightLeg_[1] = B00110001;
  hitPatternsRightLeg_[2] = B00110001;
   
  hitPatternsLeftArm_[0] = B01010101;
  hitPatternsLeftArm_[1] = B11111111;
  hitPatternsLeftArm_[2] = B10101111;   
  
  hitPatternsRightArm_[0] = B01010101;
  hitPatternsRightArm_[1] = B01000100;
  hitPatternsRightArm_[2] = B01010100; 
  
  byte posPattLeftArm[nbPatterns_][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 2, 2, 2, 2}
  };
  
  byte posPattRightArm[nbPatterns_][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  };

  byte pattSeq[nbBeats_] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2};

  setPosPatternLeftArm(posPattLeftArm);
  setPosPatternRightArm(posPattRightArm);
  setPatternSequence(pattSeq);

}
