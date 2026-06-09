#include <Arduino.h>
#include "rotarydial/RotaryDial.h"
#include "Debug.h"
#include "Pins.h"
#include "phone/Phone.h"


// ----------------- Setup ----------------- //

void setupPins();
void setLed(bool an);
void blinkLed(int dauer);

void setup() {
  Serial.begin(115200);
  
  setupPins();
  setupPhone();
}

void setupPins() {
  DEBUG_PRINTLN("System startet");
  pinMode(PIN_LED, OUTPUT);
}

// ----------------- Main ----------------- //

void loop() {
  blinkLed(500);
  
  updatePhone();



}


// ----------------- Funktion ----------------- //


void setLed(bool an) {
  digitalWrite(PIN_LED, an ? HIGH : LOW);
}

void blinkLed(int dauer) {
  static unsigned long lastChange = 0;
  static bool ledState = false;

  unsigned long now = millis();

  if (now - lastChange >= dauer) {
    ledState = !ledState;
    setLed(ledState);
    lastChange = now;
  }
}

