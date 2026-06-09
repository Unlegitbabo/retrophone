#include <Arduino.h>

#include "Pins.h"
#include "audio/audioIn/AudioIn.h"

static int audioInRaw = 0;
static unsigned long lastPrintTime = 0;

void setupAudioIn() {
  pinMode(PIN_MIC_ADC, INPUT);
  analogReadResolution(12);

  Serial.println("AudioIn ready");
}

void updateAudioIn() {
  audioInRaw = analogRead(PIN_MIC_ADC);

  unsigned long now = millis();

  if (now - lastPrintTime >= 50) {
    Serial.print(">mic:");
    Serial.println(audioInRaw);

    Serial.print(">mid:");
    Serial.println(2048);

    lastPrintTime = now;
  }
}

int getAudioInRaw() {
  return audioInRaw;
}