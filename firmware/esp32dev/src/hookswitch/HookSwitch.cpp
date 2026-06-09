#include <Arduino.h>

#include "Debug.h"
#include "Pins.h"
#include "hookswitch/HookSwitch.h"

// The switch is wired between PIN_HOOK and GND with the internal pull-up, so an
// open switch reads HIGH. Set the level that corresponds to the handset being
// LIFTED. If your cradle reads inverted on hardware, flip this to LOW.
static const int HOOK_OFFHOOK_LEVEL = HIGH;

// A cradle switch bounces mechanically; 50 ms is plenty and far slower than the
// rotary-dial pulses, so it never eats a digit.
static const unsigned long debounceMs = 50;

static bool offHook = false;
static bool justLifted = false;
static bool justPlaced = false;

static int lastRawReading = LOW;
static unsigned long lastChangeTime = 0;

void setupHookSwitch() {
  pinMode(PIN_HOOK, INPUT_PULLUP);

  // Seed the debounced state from the actual pin so we don't emit a spurious
  // edge on the first update.
  bool raw = (digitalRead(PIN_HOOK) == HOOK_OFFHOOK_LEVEL);
  offHook = raw;
  lastRawReading = raw;
  lastChangeTime = millis();

  DEBUG_PRINTLN("HookSwitch setup fertig");
}

void updateHookSwitch() {
  justLifted = false;
  justPlaced = false;

  bool raw = (digitalRead(PIN_HOOK) == HOOK_OFFHOOK_LEVEL);
  unsigned long now = millis();

  if (raw != lastRawReading) {
    lastChangeTime = now;
    lastRawReading = raw;
  }

  if (now - lastChangeTime >= debounceMs && raw != offHook) {
    offHook = raw;
    justLifted = offHook;
    justPlaced = !offHook;
  }
}

bool isOffHook() {
  return offHook;
}

bool hookJustLifted() {
  return justLifted;
}

bool hookJustPlaced() {
  return justPlaced;
}
