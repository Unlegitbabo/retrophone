#include <Arduino.h>

#include "Pins.h"
#include "phone/Phone.h"
#include "audio/audioOut/AudioOut.h"
#include "audio/audioIn/AudioIn.h"
#include "rotarydial/RotaryDial.h"
#include "audio/melodies/melodies.h"

enum PhoneState {
  PHONE_INIT,
  PHONE_IDLE,

  PHONE_0,
  PHONE_1,
  PHONE_2,
  PHONE_3,
  PHONE_4,
  PHONE_5,
  PHONE_6,
  PHONE_7,
  PHONE_8,
  PHONE_9,

  PHONE_DIALING,
  PHONE_PLAYING,
  PHONE_ERROR
};

static PhoneState phoneState = PHONE_INIT;

static bool initStarted = false;



static bool phoneStateSwitched = false;

static void changePhoneState(PhoneState newState);
static void setPhoneStateFromDigit(int digit);
static void updatePhoneState();
static void printStateOnce(const char* stateName);

void setupPhone() {
  setupAudioOut();
  setupAudioIn();
  setupRotaryDial();
}

void updatePhone() {
  readRotaryDial();

  if (isDialNumberReady()) {
    int dialNumber = getDialNumber();

    Serial.print("New Dial Number is: ");
    Serial.println(dialNumber);

    if (dialNumber >= 0 && dialNumber <= 9) {
      setPhoneStateFromDigit(dialNumber);
    } else {
      changePhoneState(PHONE_ERROR);
    }
  }


  updateMelodyPlayer();
  updatePhoneState();
  
  updateAudioIn();
 
}

static void changePhoneState(PhoneState newState) {
  if (phoneState == newState) {
    return;
  }

  phoneState = newState;
  phoneStateSwitched = false;
}

static void setPhoneStateFromDigit(int digit) {
  switch (digit) {
    case 0:
      changePhoneState(PHONE_0);
      break;

    case 1:
      changePhoneState(PHONE_1);
      break;

    case 2:
      changePhoneState(PHONE_2);
      break;

    case 3:
      changePhoneState(PHONE_3);
      break;

    case 4:
      changePhoneState(PHONE_4);
      break;

    case 5:
      changePhoneState(PHONE_5);
      break;

    case 6:
      changePhoneState(PHONE_6);
      break;

    case 7:
      changePhoneState(PHONE_7);
      break;

    case 8:
      changePhoneState(PHONE_8);
      break;

    case 9:
      changePhoneState(PHONE_9);
      break;

    default:
      changePhoneState(PHONE_ERROR);
      break;
  }
}

static void printStateOnce(const char* stateName) {
  if (!phoneStateSwitched) {
    Serial.print("State ");
    Serial.println(stateName);
    phoneStateSwitched = true;
  }
}

static void updatePhoneState() {
  switch (phoneState) {
    case PHONE_INIT:
      printStateOnce("PHONE_INIT");

      if (!initStarted) {
        setMelody(MELODY_STARTUP, false);
        initStarted = true;
      }

      if (!isMelodyActive()) {
        changePhoneState(PHONE_IDLE);
      }
      break;

    case PHONE_IDLE:
      printStateOnce("PHONE_IDLE");
      setMelody(MELODY_NONE, true);
      break;

    case PHONE_0:
      printStateOnce("PHONE_0");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_1:
      printStateOnce("PHONE_1");
      setMelody(MELODY_TETRIS, true);
      break;

    case PHONE_2:
      printStateOnce("PHONE_2");
      setMelody(MELODY_MEGALO, true);
      break;

    case PHONE_3:
      printStateOnce("PHONE_3");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_4:
      printStateOnce("PHONE_4");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_5:
      printStateOnce("PHONE_5");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_6:
      printStateOnce("PHONE_6");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_7:
      printStateOnce("PHONE_7");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_8:
      printStateOnce("PHONE_8");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_9:
      printStateOnce("PHONE_9");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_DIALING:
      printStateOnce("PHONE_DIALING");
      changePhoneState(PHONE_IDLE);
      break;

    case PHONE_PLAYING:
      printStateOnce("PHONE_PLAYING");
      break;

    case PHONE_ERROR:
      printStateOnce("PHONE_ERROR");
      changePhoneState(PHONE_IDLE);
      break;
  }
}