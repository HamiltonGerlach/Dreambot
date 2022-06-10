#include "Dreambot.h"

CtrlLED LED = CtrlLED(true);
SwitchMux Switches = SwitchMux();

void setup()
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);
    
    #if DEBUG
    Serial.begin(9600);
    #endif
}



void loop()
{
    DPRINTLN(millis());
    
    bool Changed = false;
    
    Changed = Switches.Update();
    
    DPRINTLN(Changed);
    DPRINTLN("");
    
    while (usbMIDI.read()) {}
    delay(100);
}
