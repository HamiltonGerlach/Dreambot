#include "Arduino.h"
#include "74HC4051.h"

void _74HC4051_Select(int channel) {
    digitalWrite(_74HC4051_PIN1, HIGH && (channel & B00000001));
    digitalWrite(_74HC4051_PIN2, HIGH && (channel & B00000010));
    digitalWrite(_74HC4051_PIN3, HIGH && (channel & B00000100));

    delayMicroseconds(_74HC4051_DELAY);
}
