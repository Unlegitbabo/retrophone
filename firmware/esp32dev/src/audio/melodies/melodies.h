#pragma once

enum MelodyId {
  MELODY_NONE,
  MELODY_TETRIS,
  MELODY_MEGALO,
  MELODY_STARTUP
};

void setMelody(MelodyId melodyId, bool loop);
void updateMelodyPlayer();
bool isMelodyActive();
