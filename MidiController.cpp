#include "Arduino.h"
#include "Controller.h"
#include "MidiController.h"
#include "MidiOutIf.h"
#include "Log.h"
#include "Timer.h"


MidiController::MidiController() {
  ValueStart = 0;
  ValueEnd = 0;
  ValueCurrent = 0;
  ValuePrev = 0;
  
  RampParameter = 0;
  IsRamping = false;
}

void MidiController::OnReceiveCC() {
  #if DEBUG
    LogCC('R', Channel, CN, CV);
  #endif
  // ADD_STATE(State, Iridium::StateMode::ManualMode);
}


void MidiController::OnReceivePC() {
  #if DEBUG
    LogPC('R', Channel, PN);
  #endif
}


void MidiController::OnResetCtrl() {
  // if (State & 1234) {;}
  // RESET STATE
  RST_STATE(State);
}


void MidiController::Invoke() {
  // if (IsRamping)
  // {    
  //   if (Gate.Check(IRIDIUM_RAMP_GATE_MS))
  //   {
  //     unsigned long DeltaT = millis() - Clock.TimerStart;
  //     float FacT = ((float)DeltaT / (float)IRIDIUM_RAMP_GRAD_MS);
  //     float CV = ValueStart + FacT * (ValueEnd - ValueStart);
  // 
  //     ValueCurrent = CV;
  // 
  //     if (((RampDirection == 1) && (ValueCurrent >= ValueEnd)) || 
  //        ((RampDirection == -1) && (ValueCurrent <= ValueEnd)) ||
  //        (ValueCurrent < 0) || (ValueCurrent > 127))
  //     {
  //       ValueCurrent = ValueEnd;
  //       IsRamping = false;
  //     }
  // 
  //     if ((ValueCurrent != ValueStart) && (ValueCurrent != ValuePrev))
  //     {
  //       MidiOutIf::CC(Com, Channel, RampParameter, ValueCurrent);
  //       ValuePrev = ValueCurrent;
  //     }
  // 
  //     Gate.Reset();
  //   }
  // 
  // }
}
