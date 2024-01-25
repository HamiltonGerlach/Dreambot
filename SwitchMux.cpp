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


bool SwitchMux::Update(SwitchRecord *Out)
{
    bool Changed = false;
    for (int i = 0; i < SWITCH_MUX_CTRL_CHANNELS; i++)
    {
        _74HC4051_Select(i);
        
        #if SWITCH_MUX_DEBUG
            DPRINT("CHANNEL ");
            DPRINTLN(i);
        #endif
        
        for (int j = 0; j < SWITCH_MUX_CTRL_NUM; j++)
        {
            Switches[j][i].update();
            
            bool CurrentSwitchChanged = Switches[j][i].risingEdge() | Switches[j][i].fallingEdge();
            
            Changed |= CurrentSwitchChanged;
            
            Out->Swt[j][i].changed = CurrentSwitchChanged;
            Out->Swt[j][i].state = Switches[j][i].getState();
            Out->Swt[j][i].risingEdge = Switches[j][i].risingEdge();
            Out->Swt[j][i].fallingEdge = Switches[j][i].fallingEdge();
            Out->Swt[j][i].ID = Switches[j][i].ID;
            
            #if SWITCH_MUX_DEBUG
                DPRINT("Controller ");
                DPRINT(j);
                DPRINT(", ");
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
                
            #endif
        }
        
        #if SWITCH_MUX_DEBUG
            DPRINTLN("");
        #endif
    }
    
    return Changed;
}



void SwitchMux::Reset()
{
    for (int i = 0; i < SWITCH_MUX_CTRL_CHANNELS; i++)
    {
        _74HC4051_Select(i);
        for (int j = 0; j < SWITCH_MUX_CTRL_NUM; j++)
        {
            Switches[j][i].reset();
        }
    }
}