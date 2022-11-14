#include "midi.h"
#include "note_function.h"
#include "state.h"
#include "trellis.h"
#include <usbmidi.h>

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

/* BEGIN GLOBALS */
NoteFunction note1 = NoteFunction(1, 6, 2);
/* END GLOBALS */

void
setup()
{
  randomSeed(analogRead(0));
  Serial.begin(500000);
  if (!trellis.begin()) {
    Serial.println("Could not start trellis, check wiring?");
    while (1)
      delay(1);
  } else {
    Serial.println("NeoPixel Trellis started");
  }
  initTrellisAnimation();
  delay(200);
  clearBoard();
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
