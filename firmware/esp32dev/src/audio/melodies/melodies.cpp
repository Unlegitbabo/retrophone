#include "audio/melodies/melodies.h"
#include "audio/AudioOut.h"

// ------------- Note Frequencies -------------

constexpr int REST = 0;

constexpr int C4  = 262;
constexpr int CS4 = 277;
constexpr int D4  = 294;
constexpr int DS4 = 311;
constexpr int E4  = 330;
constexpr int F4  = 349;
constexpr int FS4 = 370;
constexpr int G4  = 392;
constexpr int GS4 = 415;
constexpr int A4  = 440;
constexpr int AS4 = 466;
constexpr int B4  = 494;

constexpr int C5  = 523;
constexpr int CS5 = 554;
constexpr int D5  = 587;
constexpr int DS5 = 622;
constexpr int E5  = 659;
constexpr int F5  = 698;
constexpr int FS5 = 740;
constexpr int G5  = 784;
constexpr int GS5 = 831;
constexpr int A5  = 880;
constexpr int AS5 = 932;
constexpr int B5  = 988;
constexpr int D6  = 1175;
constexpr int C6 = 1047;

// ------------- Rhythm -------------

constexpr unsigned long BPM = 170;

constexpr unsigned long Q = 60000 / BPM;
constexpr unsigned long E = Q / 2;
constexpr unsigned long S = Q / 4;

constexpr unsigned long HIT = S;
constexpr unsigned long HOLD = E;
constexpr unsigned long GAP = 14;
constexpr unsigned long REST_SHORT = S;

// ------------- TETRIS ------------- //

static const MelodyNote TETRIS_THEME[] = {
  {659, 300}, {494, 150}, {523, 150}, {587, 300},
  {523, 150}, {494, 150}, {440, 300}, {440, 150},
  {523, 150}, {659, 300}, {587, 150}, {523, 150},
  {494, 450}, {523, 150}, {587, 300}, {659, 300},
  {523, 300}, {440, 300}, {440, 300},

  {0, 300},

  {587, 450}, {698, 150}, {880, 300}, {784, 150},
  {698, 150}, {659, 450}, {523, 150}, {659, 300},
  {587, 150}, {523, 150}, {494, 300}, {494, 150},
  {523, 150}, {587, 300}, {659, 300}, {523, 300},
  {440, 300}, {440, 300},

  {0, 500}
};

static const int TETRIS_THEME_LENGTH =
  sizeof(TETRIS_THEME) / sizeof(TETRIS_THEME[0]);

// ------------- Startup ------------- //
static const MelodyNote STARTUP_THEME[] = {
  {C5, 90}, {E5, 90}, {G5, 120},
  {C6, 180},
  {REST, 120}
};

static const int STARTUP_THEME_LENGTH =
  sizeof(STARTUP_THEME) / sizeof(STARTUP_THEME[0]);

// ------------- Megalovia ------------- //
static const MelodyNote MEGALO_THEME[] = {
  {D5, HIT}, {REST, GAP},
  {D5, HIT}, {REST, GAP},
  {D6, HOLD}, {REST, REST_SHORT},

  {A5, HIT}, {REST, GAP},
  {GS5, HIT}, {REST, GAP},
  {G5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {D5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {G5, HOLD}, {REST, REST_SHORT},

  {C5, HIT}, {REST, GAP},
  {C5, HIT}, {REST, GAP},
  {D6, HOLD}, {REST, REST_SHORT},

  {A5, HIT}, {REST, GAP},
  {GS5, HIT}, {REST, GAP},
  {G5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {D5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {G5, HOLD}, {REST, REST_SHORT},

  {B4, HIT}, {REST, GAP},
  {B4, HIT}, {REST, GAP},
  {D6, HOLD}, {REST, REST_SHORT},

  {A5, HIT}, {REST, GAP},
  {GS5, HIT}, {REST, GAP},
  {G5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {D5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {G5, HOLD}, {REST, REST_SHORT},

  {AS4, HIT}, {REST, GAP},
  {AS4, HIT}, {REST, GAP},
  {D6, HOLD}, {REST, REST_SHORT},

  {A5, HIT}, {REST, GAP},
  {GS5, HIT}, {REST, GAP},
  {G5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {D5, HIT}, {REST, GAP},
  {F5, HIT}, {REST, GAP},
  {G5, Q},

  {REST, Q}
};

static const int MEGALO_THEME_LENGTH =
  sizeof(MEGALO_THEME) / sizeof(MEGALO_THEME[0]);







static MelodyId activeMelody = MELODY_NONE;
static bool activeLoop = false;


static void applyMelodySoundProfile(MelodyId melodyId) {
  switch (melodyId) {
    case MELODY_TETRIS:
      setSoundProfile(SOUND_SINE);
      break;

    case MELODY_MEGALO:
      setSoundProfile(SOUND_8BIT);
      break;

    case MELODY_STARTUP:
        setSoundProfile(SOUND_SINE);
        break;

    case MELODY_NONE:
    default:
      setSoundProfile(SOUND_SINE);
      break;
  }
}

static void startSelectedMelody(MelodyId melodyId) {
  switch (melodyId) {
    case MELODY_TETRIS:
        setSoundProfile(SOUND_SINE);
        startMelody(TETRIS_THEME, TETRIS_THEME_LENGTH);
        break;

    case MELODY_MEGALO:
        setSoundProfile(SOUND_8BIT);
        startMelody(MEGALO_THEME, MEGALO_THEME_LENGTH);
        break;
    case MELODY_STARTUP:
        startMelody(STARTUP_THEME, STARTUP_THEME_LENGTH);
        break;

    case MELODY_NONE:
    default:
      break;
  }
}


void setMelody(MelodyId melodyId, bool loop) {
  if (activeMelody == melodyId && activeLoop == loop) {
    return;
  }

  activeMelody = melodyId;
  activeLoop = loop;

  stopMelody();

  if (activeMelody == MELODY_NONE) {
    applyMelodySoundProfile(activeMelody);
    return;
  }

  applyMelodySoundProfile(activeMelody);
  startSelectedMelody(activeMelody);
}

void updateMelodyPlayer() {
  if (activeMelody == MELODY_NONE) {
    return;
  }

  updateMelody();

  if (isMelodyPlaying()) {
    return;
  }

  if (activeLoop) {
    startSelectedMelody(activeMelody);
    return;
  }

  activeMelody = MELODY_NONE;
}

bool isMelodyActive() {
  return activeMelody != MELODY_NONE;
}