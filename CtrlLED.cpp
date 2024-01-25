#include "Arduino.h"
#include "CtrlLED.h"


const float PWM_MAX_RECPRHEX = PWM_MAX / 255.0f;


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
    
    this->Attenuation.R = 1.0f;
    this->Attenuation.G = 1.0f;
    this->Attenuation.B = 1.0f;
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
    
    this->CtrlPWM[PwmNumR].setPin(PinNumR, (int)round(PWM_MAX * Red * this->Attenuation.R), true);
    this->CtrlPWM[PwmNumG].setPin(PinNumG, (int)round(PWM_MAX * Green * this->Attenuation.G), true);
    this->CtrlPWM[PwmNumB].setPin(PinNumB, (int)round(PWM_MAX * Blue * this->Attenuation.B), true);
}


void CtrlLED::SetRGB(int NumLED, unsigned int RGB)
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
    
    float r, g, b;
    
    r = Red(RGB);
    g = Green(RGB);
    b = Blue(RGB);
    
    this->CtrlPWM[PwmNumR].setPin(PinNumR, (int)round(PWM_MAX_RECPRHEX * r * this->Attenuation.R), true);
    this->CtrlPWM[PwmNumG].setPin(PinNumG, (int)round(PWM_MAX_RECPRHEX * g * this->Attenuation.G), true);
    this->CtrlPWM[PwmNumB].setPin(PinNumB, (int)round(PWM_MAX_RECPRHEX * b * this->Attenuation.B), true);
}




void CtrlLED::SetAttenuation(float Red, float Green, float Blue)
{
    this->Attenuation.R = pow(Red, 2);
    this->Attenuation.G = pow(Green, 2);
    this->Attenuation.B = pow(Blue, 2);
}



inline int CalcPinAbs(int NumLED, PinLED ColorPin) { return (NumLED * RGB_PINS) + ColorPin; }
inline int CalcPwmNum(int PinAbs) { return (int)floor(PinAbs / PWM_PINS); }
inline int CalcPinNum(int PinAbs) { return (PinAbs % PWM_PINS); }

byte Red(unsigned int Hex) { return (Hex & 0xFF0000) >> 16; }
byte Green(unsigned int Hex) { return (Hex & 0x00FF00) >> 8; }
byte Blue(unsigned int Hex) { return (Hex & 0x0000FF); }

unsigned int HexRGB(byte R, byte G, byte B) { return ((R << 16) + (G << 8) + B); }