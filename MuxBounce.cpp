#include <Arduino.h>
#include "MuxBounce.h"


unsigned long MuxBounce::duration() {   return millis() - previous_millis; }
int MuxBounce::read()               {    return (int)state;                 }
MuxBounce::MuxBounce()              {   return; }

void MuxBounce::rebounce(unsigned long interval) {  this->rebounce_millis = interval; }

MuxBounce::MuxBounce(uint8_t pin, unsigned long interval_millis, int ID)
{
    interval(interval_millis);
    previous_millis = millis();
    state = !digitalRead(pin);
    this->pin = pin;
    this->ID = ID;
}


void MuxBounce::interval(unsigned long interval_millis)
{
    this->interval_millis = interval_millis;
    this->rebounce_millis = 0;
}


int MuxBounce::update()
{
    if (debounce()) {
        rebounce(0);
        
        stateChangeLatched = 1 + !state;
        return stateChanged = 1;
    }

    // We need to rebounce, so simulate a state change
    if (rebounce_millis && (millis() - previous_millis >= rebounce_millis)) {
        previous_millis = millis();
        rebounce(0);
         
        stateChangeLatched = 1 + !state;
        return stateChanged = 1;
    }

    return stateChanged = 0;
}


// Protected: debounces the pin
int MuxBounce::debounce()
{
    uint8_t newState = !digitalRead(pin);
    if (state != newState) {
          if (millis() - previous_millis >= interval_millis) {
              previous_millis = millis();
              state = newState;
              return 1;
        }
    }
    
    return 0;
}



uint8_t MuxBounce::getState() {
    return this->state;
}
uint8_t MuxBounce::getPin() {
    return this->pin;
}
uint8_t MuxBounce::getLastStateChange() {
    return this->stateChangeLatched;
}
unsigned long MuxBounce::getLastTime() {
    return this->previous_millis;
}

// The risingEdge method is true for one scan after the de-bounced input goes from off-to-on.
bool MuxBounce::risingEdge() { return stateChanged && state; }
// The fallingEdge  method it true for one scan after the de-bounced input goes from on-to-off. 
bool MuxBounce::fallingEdge() { return stateChanged && !state; }