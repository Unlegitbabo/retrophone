#include <Arduino.h>
#include "driver/i2s.h"
#include "BluetoothA2DPSink.h"

#include "Pins.h"
#include "audio/bluetooth/BluetoothAudio.h"
#include "audio/audioOut/AudioOut.h"

#define I2S_PORT I2S_NUM_0

// The A2DP sink drives the same I2S pins as the local tone engine (BCLK/LRC/DIN
// from Pins.h). It runs its own background task that streams the received
// Bluetooth audio straight to I2S.
static BluetoothA2DPSink a2dp_sink;
static bool btActive = false;

void enterBluetoothMode() {
  if (btActive) {
    return;
  }

  // Release the raw I2S TX driver so the A2DP sink can install its own.
  i2s_driver_uninstall(I2S_PORT);

  i2s_pin_config_t pinConfig = {
    .bck_io_num   = I2S_BCLK,
    .ws_io_num    = I2S_LRC,
    .data_out_num = I2S_DIN,
    .data_in_num  = I2S_PIN_NO_CHANGE
  };

  a2dp_sink.set_pin_config(pinConfig);
  a2dp_sink.start("RetroPhone");

  btActive = true;
  Serial.println("Bluetooth A2DP started as 'RetroPhone'");
}

void exitBluetoothMode() {
  if (!btActive) {
    return;
  }

  a2dp_sink.end(true);            // stop A2DP and free its resources
  i2s_driver_uninstall(I2S_PORT); // guarantee a clean slate (error is harmless)
  btActive = false;

  setupAudioOut();                // reinstall the local tone/melody I2S driver
  Serial.println("Bluetooth stopped, local audio restored");
}

bool isBluetoothActive() {
  return btActive;
}
