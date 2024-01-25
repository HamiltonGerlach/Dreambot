#include "Dreambot.h"
#include "MidiController.h"
#include "MidiBuffer.h"
#include <MIDI.h>


// LCD test
#include <LiquidCrystal_I2C.h>
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

byte ControlValues[SWITCH_MUX_CTRL_NUM * SWITCH_MUX_CTRL_CHANNELS];

CtrlLED LED = CtrlLED(true);
SwitchMux Switches = SwitchMux();

MidiController CtrlMIDI;
MIDI_CREATE_INSTANCE(HardwareSerial, MIDI_SERIAL, MIDI);

SwitchRecord SwtInfo;

bool isRecording = false;
bool isPlaying = false;
bool redrawOctave = false;
byte CtrlMode = 0;
int Octave = 0;

char Modes[3][10] = {
    "General  ",
    "Keyboard ",
    "Session  "
};

void setup()
{
    // LCD test routine
    lcd.init();      
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("D R E A M I N G");
    lcd.setCursor(0, 1);
    lcd.print("zzzzzzz.....");

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
    MIDI.begin(MIDI_CHANNEL_OMNI);
    
    LED.SetAttenuation(0.515f, 0.115f, 0.25f);
    LED.Flush();
    
    Switches.Reset();
    
    lcd.setCursor(0, 0);
    lcd.print("MODE: General  ");
    lcd.setCursor(0, 1);
    char strnum[2];
    char strcon[16] = "";
    sprintf(strnum, "%d", Octave);
    strcat(strcon, "OCTAVE: "); strcat(strcon, strnum); strcat(strcon, "      ");
    lcd.print(strcon);
    
    delay(1000);
    LED.Flush();
}



void loop()
{
    bool Changed = false;
    Changed = Switches.Update(&SwtInfo);
    
    
    if (Changed) {
        for (int i = 0; i < SWITCH_MUX_CTRL_CHANNELS; i++)
        {
            for (int j = 0; j < SWITCH_MUX_CTRL_NUM; j++)
            {
                if (SwtInfo.Swt[j][i].changed)
                {
                    byte swId = SwtInfo.Swt[j][i].ID;
                    if (SwtInfo.Swt[j][i].fallingEdge)
                    {
                        if ((swId < 17) && (swId > 3))
                        {
                            LED.SetRGB(swId-1, 0.0f, 0.0f, 0.0f);
                        }
                        
                        if ((swId == 16) || (swId == 14) || (swId == 12) || (swId == 11))
                        {
                            usbMIDI.sendControlChange(swId / 2 + 10, 0, 1);
                            DPRINT(swId / 2 + 10); DPRINT(' ');
                        }
                    }
                    if (SwtInfo.Swt[j][i].risingEdge)
                    {
                        
                        
                        if ((swId <= 16) && (swId >= 4))
                        {
                            LED.SetRGB(swId-1, 1.0f, 1.0f, 1.0f);
                        }
                        
                        if ((swId == 15) || (swId == 13) || (swId == 10) || (swId == 8) || (swId == 6))
                        {
                            usbMIDI.sendProgramChange(swId / 2 + 10, 1);
                            DPRINT(swId / 2 + 10); DPRINT(" PC ");
                        }
                        
                        if ((swId == 16) || (swId == 14) || (swId == 12) || (swId == 11))
                        {
                            usbMIDI.sendControlChange(swId / 2 + 10, 127, 1);
                            DPRINT(swId / 2 + 10); DPRINT(' ');
                        }
                        
                        if ((swId == 9) || (swId == 7) || (swId == 5) || (swId == 4))
                        {
                            ControlValues[swId] = (ControlValues[swId] == 127) ? 0 : 127;
                            usbMIDI.sendControlChange(swId / 2 + 10, ControlValues[swId], 1);
                            DPRINT(swId / 2 + 10);
                        }
                        
                        
                        // Dreambot mode switch
                        if ((swId == 0))
                        {
                            CtrlMode = ++CtrlMode % 3;
                            lcd.setCursor(6, 0);
                            lcd.print(Modes[CtrlMode]);
                        }
                        
                        // Handle octave up/down
                        if (swId == 18)
                        {
                            Octave = (Octave < 2) ? ++Octave : 2;
                            redrawOctave = true;
                        }
                        if (swId == 17)
                        {
                            Octave = (Octave > -2) ? --Octave : -2;
                            redrawOctave = true;
                        }
                        
                        if (redrawOctave) {
                            char strnum[2] = "  ";
                            lcd.setCursor(8, 1);
                            lcd.print(strnum);
                            sprintf(strnum, "%d", Octave);
                            lcd.setCursor(8, 1);
                            lcd.print(strnum);
                            lcd.print("         ");
                            redrawOctave = false;
                        }
                        
                        // Ableton transport (Play/Rec/Stop)
                        if ((swId == 1))
                        {
                            usbMIDI.sendControlChange(23, 127, 1);
                            LED.SetRGB(1, 0.0f, 0.0f, 0.0f);
                            LED.SetRGB(2, 0.0f, 0.0f, 0.0f);
                            
                            isRecording = false;
                            isPlaying = false;
                        }
                        if ((swId == 2))
                        {
                            usbMIDI.sendControlChange(25, 127, 1);
                            if (isRecording)
                            {
                                LED.SetRGB(1, 0.0f, 0.0f, 0.0f);
                                isRecording = false;
                            }
                            else
                            {
                                LED.SetRGB(1, 1.0f, 0.0f, 0.0f);
                                LED.SetRGB(2, 0.0f, 1.0f, 0.0f);
                                isPlaying = true;
                                isRecording = true;
                            }
                        }
                        if ((swId == 3))
                        {
                            usbMIDI.sendControlChange(24, 127, 1);
                            LED.SetRGB(2, 0.0f, 1.0f, 0.0f);
                            isPlaying = true;
                            // ToDo: queue flash if is already playing
                        }
                    }
                }
            }
        }
    }
    
    
    // Handle MIDI thru
    while (MIDI.read())
    {
        byte b1, b2, type;
        int channel;
        type = MIDI.getType();
        channel = MIDI.getChannel();
        b1 = MIDI.getData1();
        b2 = MIDI.getData2();
        
        switch (type)
        {
            case midi::NoteOn:
                DPRINTLN(String("Note On: ch=") + channel + ", note=" + b1 + ", velocity=" + b2);
                MIDI.sendNoteOn(b1, b2, 2);
                break;
            case midi::NoteOff:
                DPRINTLN(String("Note Off: ch=") + channel + ", note=" + b1 + ", velocity=" + b2);
                MIDI.sendNoteOff(b1, b2, 2);
                break;
            case midi::AfterTouchChannel:
                DPRINTLN(String("Aftertouch: ch=") + channel + ", value=" + b1);
                MIDI.sendAfterTouch(b1, 2);
                break;
            case midi::AfterTouchPoly:
                DPRINTLN(String("Aftertouch Poly: ch=") + channel + ", note=" + b1 + ", value=" + b2);
                MIDI.sendPolyPressure(b1, b2, 2);
                break;
            case midi::PitchBend:
                DPRINTLN(String("Pitch Bend: ch=") + channel + ", value=" + b1);
                MIDI.sendPitchBend(b1, 2);
                break;
            case midi::ControlChange:
                DPRINTLN(String("CC: ch=") + channel + ", cn=" + b1 + ", cv=" + b2);
                MIDI.sendControlChange(b1, b2, 2);
                break;
            case midi::ProgramChange:
                DPRINTLN(String("PC: ch=") + channel + ", pn=" + b1);
                MIDI.sendControlChange(b1, b2, 2);
                break;
            default:
                DPRINTLN(String("Message, type=") + type + ", data = " + b1 + " " + b2);
        }
    }
    
    
    /// Handle USB MIDI to MIDI thru
    while (usbMIDI.read())
    {
        byte type = usbMIDI.getType();
        int channel = usbMIDI.getChannel();
        byte b1 = usbMIDI.getData1();
        byte b2 = usbMIDI.getData2();
        int value;
        
        switch (type)
        {
            case midi::NoteOn:
                MIDI.sendNoteOn(b1, b2, channel);
                DPRINTLN(String("Note On: ch=") + channel + ", note=" + b1 + ", velocity=" + b2);
                
                break;
            case midi::NoteOff:
                MIDI.sendNoteOff(b1, b2, channel);
                DPRINTLN(String("Note Off: ch=") + channel + ", note=" + b1 + ", velocity=" + b2);
                
                break;
            case midi::ControlChange:
                MIDI.sendControlChange(b1, b2, channel);
                DPRINTLN(String("CC: ch=") + channel + ", CN=" + b1 + ", CV=" + b2);
                
                break;
            case midi::ProgramChange:
                MIDI.sendProgramChange(b1, channel);
                DPRINTLN(String("PC: ch=") + channel + ", P1=" + b1 + ", P2=" + b2);
                
                break;
            case midi::AfterTouchChannel:
                MIDI.sendAfterTouch(b1, channel);
                DPRINTLN(String("AfterTouchChannel: ch=") + channel + ", d1=" + b1 + ", d2=" + b2);
                
                break;
            case midi::AfterTouchPoly:
                MIDI.sendPolyPressure(b1, b2, channel);
                DPRINTLN(String("AfterTouchPoly: ch=") + channel + ", d1=" + b1 + ", d2=" + b2);
                
                break;
            case midi::PitchBend:
                value = (b2 << 7) + b1;
                MIDI.sendPitchBend(channel, value);
                DPRINTLN(String("Pitchbend: ch=") + channel + ", PB1=" + b1 + ", PB2=" + b2 + ", value = " + value);
                
                break;
            default:
                DPRINTLN(String("Message: ch=, type=") + channel + ", " + type + ", data = " + b1 + " " + b2);
        }
    }
}
