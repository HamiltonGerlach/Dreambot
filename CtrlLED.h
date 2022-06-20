#ifndef CTRL_LED_H
#define CTRL_LED_H

#include <Adafruit_PWMServoDriver.h>

#define PWM_BASE_ADDRESS 0x40

#define PWM_ADDRESS_0 0x40
#define PWM_ADDRESS_1 0x41
#define PWM_ADDRESS_2 0x42

#define PWM_FREQ 1000
#define OSC_FREQ 27000000

#define NUM_LED 16
#define PWM_PINS 16
#define NUM_PINS 48
#define RGB_PINS 3
#define NUM_CTRL 3

#define PWM_MAX 4095

enum PinLED { R, G, B };

class CtrlLED {
    public:
        void Flash();
        void Flush();
        
        void SetRGB(int NumLED, float Red, float Green, float Blue);
        void SetRGB(int NumLED, unsigned int RGB);
        
        CtrlLED(bool State);
    protected:
        Adafruit_PWMServoDriver CtrlPWM[RGB_PINS];
        bool State;
};


inline int CalcPinAbs(int NumLED, PinLED ColorPin);
inline int CalcPwmNum(int PinAbs);
inline int CalcPinNum(int PinAbs);

byte Red(unsigned int Hex);
byte Green(unsigned int Hex);
byte Blue(unsigned int Hex);

unsigned int HexRGB(byte R, byte G, byte B);

#endif
