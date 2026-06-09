
#include <Arduino.h>
#include "driver/i2s.h"
#include <math.h>
#include "Pins.h"
#include "AudioOut.h"

#define I2S_PORT I2S_NUM_0





static const MelodyNote* currentMelody = nullptr;
static int currentMelodyLength = 0;
static int currentMelodyIndex = 0;
static unsigned long currentNoteStartTime = 0;
static bool melodyPlaying = false;
static SoundProfile currentSoundProfile = SOUND_SINE;


// ----------------- SETUP ----------------- //
void setupAudioOut() {
  i2s_config_t i2sConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pinConfig = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  esp_err_t result;

  result = i2s_driver_install(I2S_PORT, &i2sConfig, 0, NULL);
  Serial.print("i2s_driver_install: ");
  Serial.println(result);

  result = i2s_set_pin(I2S_PORT, &pinConfig);
  Serial.print("i2s_set_pin: ");
  Serial.println(result);

  result = i2s_zero_dma_buffer(I2S_PORT);
  Serial.print("i2s_zero_dma_buffer: ");
  Serial.println(result);

  result = i2s_start(I2S_PORT);
  Serial.print("i2s_start: ");
  Serial.println(result);

  Serial.print("I2S BCLK: ");
  Serial.println(I2S_BCLK);

  Serial.print("I2S LRC: ");
  Serial.println(I2S_LRC);

  Serial.print("I2S DIN: ");
  Serial.println(I2S_DIN);
}
// ----------------- Funktion ----------------- //

void playTestTone() {
  const int sampleRate = 44100;
  const int frequency = 440;
  const int amplitude = 32767;
  const int samplesPerBlock = 128;

  static float phase = 0.0;
  float phaseStep = 2.0 * PI * frequency / sampleRate;

  int16_t samples[samplesPerBlock * 2];

  for (int i = 0; i < samplesPerBlock; i++) {
    int16_t sample = (int16_t)(sin(phase) * amplitude);

    samples[i * 2] = sample;
    samples[i * 2 + 1] = sample;

    phase += phaseStep;
    if (phase >= 2.0 * PI) {
      phase -= 2.0 * PI;
    }
  }

  size_t bytesWritten = 0;
  esp_err_t result = i2s_write(
    I2S_PORT,
    samples,
    sizeof(samples),
    &bytesWritten,
    portMAX_DELAY
  );

  static unsigned long lastDebugTime = 0;
  unsigned long now = millis();

  if (now - lastDebugTime >= 500) {
    Serial.print("i2s_write result: ");
    Serial.println(result);

    Serial.print("bytesWritten: ");
    Serial.println(bytesWritten);

    lastDebugTime = now;
  }
}


static void writeToneBlock(int frequency, int amplitude) {
  const int sampleRate = 44100;
  const int samplesPerBlock = 128;

  static float phase = 0.0;
  float phaseStep = 2.0 * PI * frequency / sampleRate;

  int16_t samples[samplesPerBlock * 2];

  for (int i = 0; i < samplesPerBlock; i++) {
    int16_t sample = 0;

    if (frequency > 0) {
      switch (currentSoundProfile) {
        case SOUND_8BIT:
            sample = phase < PI ? amplitude * 2 / 10 : -amplitude * 2 / 10;
            break;

        case SOUND_RADIO:
          sample = (int16_t)(sin(phase) * amplitude);
          sample = sample / 2;
          break;

        case SOUND_SINE:
        default:
          sample = (int16_t)(sin(phase) * amplitude);
          break;
      }

      phase += phaseStep;
      if (phase >= 2.0 * PI) {
        phase -= 2.0 * PI;
      }
    }

    samples[i * 2] = sample;
    samples[i * 2 + 1] = sample;
  }

  size_t bytesWritten = 0;
  i2s_write(I2S_PORT, samples, sizeof(samples), &bytesWritten, portMAX_DELAY);
}

void playToneSequence() {
  struct ToneStep {
    int frequency;
    unsigned long durationMs;
  };

  static const ToneStep sequence[] = {
    {440, 300},
    {0, 150},
    {660, 300},
    {0, 150},
    {880, 300},
    {0, 500}
  };

  static int stepIndex = 0;
  static unsigned long stepStartTime = 0;

  unsigned long now = millis();

  if (stepStartTime == 0) {
    stepStartTime = now;
  }

  if (now - stepStartTime >= sequence[stepIndex].durationMs) {
    stepIndex++;

    if (stepIndex >= (int)(sizeof(sequence) / sizeof(sequence[0]))) {
      stepIndex = 0;
    }

    stepStartTime = now;
  }

  writeToneBlock(sequence[stepIndex].frequency, 12000);
}


void startMelody(const MelodyNote* melody, int length) {
  if (melody == nullptr || length <= 0) {
    melodyPlaying = false;
    return;
  }

  currentMelody = melody;
  currentMelodyLength = length;
  currentMelodyIndex = 0;
  currentNoteStartTime = millis();
  melodyPlaying = true;
}

bool isMelodyPlaying() {
  return melodyPlaying;
}

void updateMelody() {
  if (!melodyPlaying) {
    return;
  }

  MelodyNote note = currentMelody[currentMelodyIndex];

  unsigned long now = millis();

  if (now - currentNoteStartTime >= note.durationMs) {
    currentMelodyIndex++;

    if (currentMelodyIndex >= currentMelodyLength) {
      melodyPlaying = false;
      return;
    }

    currentNoteStartTime = now;
    note = currentMelody[currentMelodyIndex];
  }

  writeToneBlock(note.frequency, 32767);
}

void stopMelody() {
  melodyPlaying = false;
  currentMelody = nullptr;
  currentMelodyLength = 0;
  currentMelodyIndex = 0;
  currentNoteStartTime = 0;

  writeToneBlock(0, 0);
}

void setSoundProfile(SoundProfile profile) {
  currentSoundProfile = profile;
}

SoundProfile getSoundProfile() {
  return currentSoundProfile;
}