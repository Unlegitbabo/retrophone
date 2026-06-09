#pragma once

struct MelodyNote {
  int frequency;
  unsigned long durationMs;
};

void setupAudioOut();
void playTestTone();
void stopAudioOut();
void playToneSequence();

void setupAudioFiles();
void playWavFile(const char* path);

void startMelody(const MelodyNote* melody, int length);
void updateMelody();
bool isMelodyPlaying();
void stopMelody();


enum SoundProfile {
  SOUND_SINE,
  SOUND_8BIT,
  SOUND_RADIO
};

void setSoundProfile(SoundProfile profile);
SoundProfile getSoundProfile();