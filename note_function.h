#ifndef NoteFunction_h
#define NoteFunction_h
#include "state.h"
#include <usbmidi.h>
char notes[12][3] = { "c",  "c#", "d",  "d#", "e",  "f",
                      "f#", "g",  "g#", "a",  "a#", "b" };
class NoteFunction
{
private:
  uint8_t channel;
  uint8_t every;
  uint8_t note;
  uint8_t prevNote;
  uint8_t octave;
  uint8_t seq[16];
  uint8_t index;

public:
  NoteFunction(uint8_t channel, uint8_t every, uint8_t octave)
  {
    this->channel = channel;
    this->every = every;
    this->note = 0;
    this->prevNote = 0;
    for (uint8_t i = 0; i < 16; i++) {
      this->seq[i] = 1;
    }
    this->octave = octave;
  }

  void pulse(AppState* appState)
  {
    if (appState->pulses % this->every == 0) {
      if (this->seq[this->index] == 1) {
        this->note = getRandomNote();
        sendNoteOff(this->channel, this->prevNote);
        sendNoteOn(this->channel, this->note, 127);
        this->prevNote = this->note;
      }
      if (this->index == 15) {
        this->index = 0;
      } else {
        this->index++;
      }
    }
  }
  uint8_t getRandomNumber(uint8_t min_, uint8_t max_)
  {
    return random(min_, max_);
  }

  uint8_t getRandomNote()
  {
    uint8_t plusOneOctave =
      random(this->octave, this->octave == 8 ? 8 : this->octave + 2);
    return getNote(plusOneOctave, notes[getRandomNumber(0, 12)]);
  }
  uint8_t getNote(uint8_t octave, char* note)
  {
    /*
      So this is where it gets interesting. If "middle C" is defined as C3,
      0 == C-2
      if "middle C" is defined as C4
      0 == C-1
      Ableton uses C4 AFAIK, so let's assume that for now. Make this a setting
      later.
    */
    uint8_t base = 0;
    if (*(note + 1) == '\0') {
      switch (*note) {
        case 'c':
          break;
        case 'd':
          base = 2;
          break;
        case 'e':
          base = 4;
          break;
        case 'f':
          base = 5;
          break;
        case 'g':
          base = 7;
          break;
        case 'a':
          base = 9;
          break;
        case 'b':
          base = 11;
          break;
      }
    } else {
      switch (*note) {
        case 'c':
          base = 1;
          break;
        case 'd':
          base = 3;
          break;
        case 'f':
          base = 6;
          break;
        case 'g':
          base = 8;
          break;
        case 'a':
          base = 10;
          break;
      }
    }
    Serial.println(octave);
    return base + (12 * octave);
  }
  uint8_t getNoteOn()
  {
    // assert channel <= 15;
    return 144 + this->channel;
  }

  uint8_t getNoteOff()
  {
    // assert channel <= 15;
    return 128 + this->channel;
  }
  uint8_t getCCChange()
  {
    // assert channel <= 15;
    return 176 + this->channel;
  }

  void sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
  {
    USBMIDI.write(getNoteOn());
    USBMIDI.write(note);
    USBMIDI.write(velocity);
    USBMIDI.flush();
  }
  void sendNoteOff(uint8_t channel, uint8_t note)
  {
    USBMIDI.write(getNoteOff());
    USBMIDI.write(note);
    USBMIDI.write(0); // velocity
    USBMIDI.flush();
  }
  void sendCCChange(uint8_t channel, uint8_t control, uint8_t val)
  {
    USBMIDI.write(getCCChange());
    USBMIDI.write(control);
    USBMIDI.write(val);
    USBMIDI.flush();
  }
};
#endif
