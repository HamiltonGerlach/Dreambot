#ifndef SWITCHMUX_H
#define SWITCHMUX_H

#include "Arduino.h"
#include "MuxBounce.h"

#define SWITCH_DEBOUNCE_TIME 15
#define SWITCH_MUX_CTRL_NUM 3
#define SWITCH_MUX_CTRL_CHANNELS 8

#define SWITCH_MUX_INPUT_PIN 13

#define SWITCH_MUX_DEBUG false

struct SwitchInfo {
    uint8_t state;
    bool changed;
    bool fallingEdge;
    bool risingEdge;
    byte ID;
};

struct SwitchRecord {
    SwitchInfo Swt[SWITCH_MUX_CTRL_NUM][SWITCH_MUX_CTRL_CHANNELS];
};

class SwitchMux {
    public:
        SwitchMux();
        bool Update(SwitchRecord *Out);
        void Reset();
        // GetSwitchState();
        
    protected:
        MuxBounce Switches[SWITCH_MUX_CTRL_NUM][SWITCH_MUX_CTRL_CHANNELS];
};


#endif
