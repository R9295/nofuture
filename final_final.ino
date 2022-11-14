#include "midi.h"
#include "note_function.h"
#include "state.h"
#include "trellis.h"
#include <usbmidi.h>

class HitFunction
{
private:
  uint8_t channel;
  uint8_t every;
  uint8_t min_;
  uint8_t max_;
};

/* BEGIN GLOBALS */
AppState state = AppState();
NoteFunction note1 = NoteFunction(1, 6, 2);
/* END GLOBALS */

// define a callback for key presses
TrellisCallback
handleKeyPress(keyEvent evt)
{
  // Pressed
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    uint8_t key = getIndexFromKey(evt.reg);
    state.keysPressed[key] = true;
    if (state.keysPressed[0] == true && state.keysPressed[15] == true) {
      state.toggleOptionMenu();
    }
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    uint8_t key = getIndexFromKey(evt.reg + 1);
    state.keysPressed[key] = false;
  }
  return 0;
}

void
setup()
{
  randomSeed(analogRead(0));
  Serial.begin(19200);
  if (!trellis.begin()) {
    Serial.println("Could not start trellis, check wiring?");
    while (1)
      delay(1);
  } else {
    Serial.println("NeoPixel Trellis started");
  }
  // activate all keys and set callbacks
  for (int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++) {
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, handleKeyPress);
  }
  initTrellisAnimation();
  delay(200);
  clearBoard();
}

void
loop()
{
  trellis.read();
  USBMIDI.poll();
  while (USBMIDI.available()) {
    u8 msg = USBMIDI.read();
    if (msg == CLOCK_MSG) {
      Serial.println(state.optionMenu);
      state.pulse();
      note1.pulse(&state);
    } else if (msg == STOP_MSG) {
      state.clean();
      note1.clean();
    }
  }
}
