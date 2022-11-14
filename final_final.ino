#include <usbmidi.h>
#include "midi.h"
#include "state.h"
#include "note_function.h"

/* BEGIN STATE DECLARATIONS */
APPState st = { 0, 1, 0 };
APPState* state = &st;
///* END STATE DECLARATIONS */

class HitFunction
{
private:
  uint8_t channel;
  uint8_t every;
  uint8_t min_;
  uint8_t max_;
};

NoteFunction note1 = NoteFunction(1, 6, 2);
void
setup()
{
  randomSeed(analogRead(0));
  Serial.begin(500000);
}

void
loop()
{
  USBMIDI.poll();
  while (USBMIDI.available()) {
    u8 msg = USBMIDI.read();
    if (msg == CLOCK_MSG) {
      state->pulses++;
      note1.pulse(state);
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