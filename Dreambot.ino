#include "Dreambot.h"
#include "MidiController.h"
#include "MidiBuffer.h"
#include <MIDI.h>


CtrlLED LED = CtrlLED(true);
SwitchMux Switches = SwitchMux();

MidiController CtrlMIDI;
MIDI_CREATE_INSTANCE(HardwareSerial, MIDI_SERIAL, MIDI);


void setup()
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);
    
    #if DEBUG
    Serial.begin(USB_BAUDRATE);
    #endif
    
    // MIDI I/O
    MIDI_SERIAL.setRX(MIDI_SERIAL_RX);
    MIDI_SERIAL.setTX(MIDI_SERIAL_TX);
    MIDI.begin(MIDI_BAUDRATE);
    CtrlMIDI.Init(&MIDI_SERIAL, MIDI_CHANNEL);
}



void loop()
{
    DPRINTLN(millis());
    
    bool Changed = false;
    
    Changed = Switches.Update();
    
    DPRINTLN(Changed);
    DPRINTLN("");
    
    while (MIDI.read()) {}
    while (usbMIDI.read()) {}
    delay(100);
}
