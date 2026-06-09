#pragma once

// Hook-switch: detects whether the handset is lifted off the cradle.
//
// off-hook = handset lifted  (ready to dial / answer a call)
// on-hook  = handset resting (idle / hang up)
//
// updateHookSwitch() must be called every loop. The edge queries below are
// valid for the loop iteration that follows an update() call.

void setupHookSwitch();
void updateHookSwitch();

bool isOffHook();          // current debounced state
bool hookJustLifted();     // edge: on-hook -> off-hook this update
bool hookJustPlaced();     // edge: off-hook -> on-hook this update
