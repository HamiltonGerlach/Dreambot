#ifndef MIDIBUFFER_H
#define MIDIBUFFER_H

#include "Arduino.h"

#define CONTROL_BYTE 0xFF
#define CONTROL_FLUSH true

// Midi buffer
class MidiBuffer {
  public:
    static byte Data[3];
    
    static void Push(byte In);
    static bool CCBank(byte Channel);
    static bool CCNorm(byte Channel);
    static bool CCTerm(byte Channel);
    static byte PC(byte Channel);
    static void Flush();
  
  private:
    MidiBuffer();
};

#endif
