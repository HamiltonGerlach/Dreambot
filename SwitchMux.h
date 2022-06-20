#ifndef SWITCHMUX_H
#define SWITCHMUX_H

#include "Arduino.h"
#include "MuxBounce.h"

#define SWITCH_DEBOUNCE_TIME 5
#define SWITCH_MUX_CTRL_NUM 3
#define SWITCH_MUX_CTRL_CHANNELS 8

#define SWITCH_MUX_INPUT_PIN 13

class SwitchMux {
    public:
        SwitchMux();
        bool Update();
        void Reset();
        // GetSwitchState();
        
    protected:
        MuxBounce Switches[SWITCH_MUX_CTRL_NUM][SWITCH_MUX_CTRL_CHANNELS];
};


#endif
