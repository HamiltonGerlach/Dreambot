#ifndef MIDICTRL_H
#define MIDICTRL_H

#include "Controller.h"
#include "Timer.h"

class MidiController : public Controller
{
private:
    Timer Clock, Gate;
    
    byte ValueStart, ValueEnd, ValueCurrent, ValuePrev;
    byte RampParameter, RampDirection;
    bool IsRamping;
    
    int Param, State;
        
public:    
    void OnReceiveCC();
    void OnReceivePC();
    void OnResetCtrl();
    
    void Invoke();
    
    MidiController();
};

#endif
