#include <Arduino.h>
#include "RotaryDial.h"
#include "Debug.h"
#include "Pins.h"




#define TEST_PIN 18










// ----------------- Setup ----------------- //

void setupPins();
void setLed(bool an);
void blinkLed(int dauer);

void setup() {
  Serial.begin(115200);
  setupRotaryDial();
  setupPins();
  digitalWrite(TEST_PIN, HIGH);
}

void setupPins() {
  DEBUG_PRINTLN("System startet");
  pinMode(PIN_LED, OUTPUT);
  pinMode(TEST_PIN, OUTPUT);
}

// ----------------- Main ----------------- //

void loop() {
  blinkLed(500);
  readRotaryDial();


  if (isDialNumberReady()) {
    int DialNumber = getDialNumber();

    Serial.println("New Dial Number is:");
    Serial.println(DialNumber);
  }

  if (isPhoneNumberReady()){
    int PhoneNumber = getPhoneNumber();
    
    Serial.println("New Phone Nummber is:");
    Serial.println(PhoneNumber);
  }

  //Serial.println(PhoneNumber);
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

