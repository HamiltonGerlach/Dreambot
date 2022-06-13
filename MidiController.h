#ifndef MIDICTRL_H
#define MIDICTRL_H

#include "Controller.h"
#include "Timer.h"

// #define CUSTOM_MSG_NUM 1

class MidiController : public Controller
{
private:
    Timer Clock, Gate;
    
    byte ValueStart, ValueEnd, ValueCurrent, ValuePrev;
    byte RampParameter, RampDirection;
    bool IsRamping;
    
    int Param, State;
        
public:
    byte CustomMsgIdx;
    
    // _customMsg CustomMessage[CUSTOM_MSG_NUM] = { NULL };
    
    void OnReceiveCC();
    void OnReceivePC();
    void OnResetCtrl();
    
    void Invoke();
    
    MidiController();
};

#endif
