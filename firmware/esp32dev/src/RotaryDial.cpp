#include "RotaryDial.h"
#include <Arduino.h>
#include "Debug.h"
#include "Pins.h"




static int InternDigitCounter = 0;
static int InternDigitNumber = 0;
static bool CreatDialNumber = false;
static bool DialNumberCompleted = false;


static int PhoneNumber;
static bool CreatPhoneNumberTrue = false;
static bool PhoneNumberCompleted = false;


static bool ReadActiveLastState = false;
static unsigned long lastReadActiveTime = 0;
static unsigned long lastCountTime = 0;
const unsigned long debounceMs = 100;
static int NumberLastState = HIGH;




void setupRotaryDial() {
  pinMode(PIN_DIAL_NUMBER, INPUT_PULLUP);
  pinMode(PIN_DIAL_ACTIVE, INPUT_PULLUP);
  DEBUG_PRINTLN("PhoneNumber setup fertig");

}







void readRotaryDial() {
    
    bool ReadActive = digitalRead(PIN_DIAL_ACTIVE) == LOW;

    if (ReadActiveLastState == false && ReadActive == true) {
        unsigned long now = millis();
        if (now - lastReadActiveTime >= debounceMs)  {
            InternDigitCounter = 0;
            InternDigitNumber = 0;
            CreatDialNumber = true;
            DialNumberCompleted = false;

            lastReadActiveTime = now;
        }
    }
    if (ReadActiveLastState == true && ReadActive == false) {
        unsigned long now = millis();

        if (now - lastReadActiveTime >= debounceMs) {
            if (InternDigitCounter == 10) {
                InternDigitNumber = 0;
            } else if (InternDigitCounter >= 1 && InternDigitCounter <= 9) {
                InternDigitNumber = InternDigitCounter;
            } else {
                InternDigitNumber = -1;
            }
            
            InternDigitCounter = 0;
            CreatDialNumber = false;
            DialNumberCompleted = true;
            lastReadActiveTime = now;
        }
    }

    if (ReadActive && CreatDialNumber) {

        bool NumberState = digitalRead(PIN_DIAL_NUMBER);

        if (NumberLastState == HIGH && NumberState == LOW) {
            unsigned long NumberMilis = millis();

            if (NumberMilis - lastCountTime >= debounceMs) {
                InternDigitCounter++;
                lastCountTime = NumberMilis;
            }
        }
        NumberLastState = NumberState;
    }

    ReadActiveLastState = ReadActive;

 
    #if DEBUG
        static unsigned long lastDebugTime = 0;
        unsigned long DebugMilis = millis();

        if (DebugMilis - lastDebugTime >= 500) {
            DEBUG_PRINT("Phone Number: ");
            DEBUG_PRINTLN(PhoneNumber);

            DEBUG_PRINT("Dial Number: ");
            DEBUG_PRINTLN(InternDigitNumber);

            DEBUG_PRINT("ReadActive: ");
            DEBUG_PRINTLN(ReadActive);

            lastDebugTime = DebugMilis;
        }
        #endif
}


bool isDialNumberReady(){

    if (DialNumberCompleted){
        return true;
    }
    return false;
}


int getDialNumber(){
    
    if (isDialNumberReady()) {
    DialNumberCompleted = false;
    return InternDigitNumber;
    }
return -1;
}

bool isPhoneNumberReady(){
    if (PhoneNumberCompleted) {
        return true;
    }
    return false;
}

int getPhoneNumber(){
    if (isPhoneNumberReady()) {
        PhoneNumberCompleted = false;
        return PhoneNumber;
    }
    return -1;
}
