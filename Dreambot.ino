#include "Dreambot.h"
#include "Log.h"

MuxBounce Switches[3][8];

CtrlLED LED = CtrlLED(true);

void setup()
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);

    int count = 0;
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 7; j++) {
                Switches[i][j] = MuxBounce(13 + i, 5, count++);
        }
    }
    
    #if DEBUG
    Serial.begin(9600);
    #endif
}



void loop()
{
    DPRINTLN(millis());

    bool Changed = false;
    for (int i = 0; i <= 7; i++)
    {
        _74HC4051_Select(i);
        DPRINT("CHANNEL ");
        DPRINTLN(i);
        
        for (int j = 0; j <= 2; j++)
        {
            DPRINT("Controller ");
            DPRINT(j);
            DPRINT(", ");
            Changed |= Switches[j][i].update();
            DPRINT("Pin ");
            DPRINT(Switches[j][i].getPin());
            DPRINT(": ");
            DPRINT(Switches[j][i].getState());
            DPRINT(" => ");
            DPRINT("Switch #");
            DPRINT(Switches[j][i].ID);
            if (Switches[j][i].ID < 10) { DPRINT(" "); }
            DPRINT(" - Last state change: ");
            DPRINT(Switches[j][i].getLastStateChange());
            DPRINT(" at ");
            DPRINTLN(Switches[j][i].getLastTime());
        }
        
        DPRINTLN("");
    }
    
    DPRINTLN(Changed);
    DPRINTLN("");
        
    unsigned long now = millis();
    int ctrl = (int)floor(now / 1000) % 4;
    
    switch (ctrl)
    {
        case 0:
            LED.SetRGB(12, 1.0f, 0.0f, 0.0f);
            break;
        case 1:
            LED.SetRGB(12, 0.0f, 1.0f, 0.0f);
            break;
        case 2:
            LED.SetRGB(12, 0.0f, 0.0f, 1.0f);
            break;
        case 3:
            LED.SetRGB(12, 0.0f, 0.0f, 0.0f);
            break;
    }
    
    while (usbMIDI.read()) {}
    delay(100);
}
