#include "SwitchMux.h"
#include "74HC4051.h"
#include "Log.h"

SwitchMux::SwitchMux()
{
    int count = 0;
    for (int i = 0; i < SWITCH_MUX_CTRL_NUM; i++) {
        for (int j = 0; j < SWITCH_MUX_CTRL_CHANNELS; j++) {
                this->Switches[i][j] = MuxBounce(SWITCH_MUX_INPUT_PIN + i, SWITCH_DEBOUNCE_TIME, count++);
        }
    }
}


bool SwitchMux::Update()
{
    bool Changed = false;
    for (int i = 0; i < SWITCH_MUX_CTRL_CHANNELS; i++)
    {
        _74HC4051_Select(i);
        DPRINT("CHANNEL ");
        DPRINTLN(i);
        
        for (int j = 0; j < SWITCH_MUX_CTRL_NUM; j++)
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
    
    return Changed;
}