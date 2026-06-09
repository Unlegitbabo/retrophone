#pragma once

// Bluetooth audio mode manager.
//
// The local tone/melody engine (AudioOut) and the Bluetooth stack both want to
// own the I2S peripheral, so they cannot run at once. These functions switch
// between the two modes, handing the I2S peripheral back and forth.
//
// Phase 1 implements A2DP (music streaming). HFP (phone calls) will be added
// here later as a second profile on top of the same Bluetooth stack.

void enterBluetoothMode();   // release local I2S, start the A2DP sink
void exitBluetoothMode();    // stop Bluetooth, restore local tone/melody audio
bool isBluetoothActive();
