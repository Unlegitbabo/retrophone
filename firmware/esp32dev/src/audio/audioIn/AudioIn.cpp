#include <Arduino.h>

#include "Debug.h"
#include "Pins.h"
#include "audio/audioIn/AudioIn.h"

static int audioInRaw = 0;

void setupAudioIn() {
  pinMode(PIN_MIC_ADC, INPUT);
  analogReadResolution(12);

  Serial.println("AudioIn ready");
}

void updateAudioIn() {
  audioInRaw = analogRead(PIN_MIC_ADC);

#if DEBUG
  static unsigned long lastPrintTime = 0;
  unsigned long now = millis();

  if (now - lastPrintTime >= 50) {
    Serial.print(">mic:");
    Serial.println(audioInRaw);

    Serial.print(">mid:");
    Serial.println(2048);

    lastPrintTime = now;
  }
#endif
}

int getAudioInRaw() {
  return audioInRaw;
}