#include <usbmidi.h>
#include "state.h"
#include "midi.h"

/* BEGIN STATE DECLARATIONS */
APPState st = { 0, 1, 0 };
APPState* state = &st;
///* END STATE DECLARATIONS */

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(500000);
}

void loop() {
 USBMIDI.poll();
  while (USBMIDI.available()) {
    u8 msg = USBMIDI.read();
    if (msg == CLOCK_MSG) {
      state->pulses++;
      if (state->pulses == PULSES_IN_BAR) {
        state->bars++;
        state->pulses = 0;
      }
    } else if (msg == STOP_MSG) {
       state->bars = 1;
       state->pulses = 0;
    }
  }
}
