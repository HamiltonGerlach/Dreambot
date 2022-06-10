#include "Arduino.h"
#include "CtrlLED.h"



CtrlLED::CtrlLED(bool State) {
    for (int i = 0; i < NUM_CTRL; i++)
    {
        this->CtrlPWM[i] = Adafruit_PWMServoDriver(PWM_BASE_ADDRESS + i, Wire);
    }
        
    for (int i = 0; i < RGB_PINS; i++)
    {
        this->CtrlPWM[i].begin();
        this->CtrlPWM[i].setOscillatorFrequency(OSC_FREQ);
        this->CtrlPWM[i].setPWMFreq(PWM_FREQ);
    }
    
    this->Flush();
    this->State = State;
}



void CtrlLED::Flash() {
    for (int i = 0; i < NUM_LED; i++)
    {
        this->SetRGB(i, 1.0f, 1.0f, 1.0f);
    }
    return;
}

void CtrlLED::Flush() {
    for (int i = 0; i < NUM_LED; i++)
    {
        this->SetRGB(i, 0.0f, 0.0f, 0.0f);
    }
    return;
}


void CtrlLED::SetRGB(int NumLED, float Red, float Green, float Blue)
{
    int PinAbsR = CalcPinAbs(NumLED, PinLED::R);
    int PinAbsG = CalcPinAbs(NumLED, PinLED::G);
    int PinAbsB = CalcPinAbs(NumLED, PinLED::B);
    
    int PwmNumR = CalcPwmNum(PinAbsR);
    int PinNumR = CalcPinNum(PinAbsR);
    
    int PwmNumG = CalcPwmNum(PinAbsG);
    int PinNumG = CalcPinNum(PinAbsG);
    
    int PwmNumB = CalcPwmNum(PinAbsB);
    int PinNumB = CalcPinNum(PinAbsB);
    
    this->CtrlPWM[PwmNumR].setPin(PinNumR, (int)round(PWM_MAX * Red), true);
    this->CtrlPWM[PwmNumG].setPin(PinNumG, (int)round(PWM_MAX * Green), true);
    this->CtrlPWM[PwmNumB].setPin(PinNumB, (int)round(PWM_MAX * Blue), true);
}



inline int CalcPinAbs(int NumLED, PinLED ColorPin) { return (NumLED * RGB_PINS) + ColorPin; }
inline int CalcPwmNum(int PinAbs) { return (int)floor(PinAbs / PWM_PINS); }
inline int CalcPinNum(int PinAbs) { return (PinAbs % PWM_PINS); }