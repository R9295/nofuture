#ifndef NoteFunction_h
#define NoteFunction_h
#include "state.h"
#include "trellis.h"
#include <usbmidi.h>
char notes[12][3] = { "c",  "c#", "d",  "d#", "e",  "f",
                      "f#", "g",  "g#", "a",  "a#", "b" };
// uint8_t lengths[5] = {1, 2, 4, 6, 8};
class NoteFunction
{
private:
  uint8_t channel;
  uint8_t every;
  uint8_t note;
  uint8_t note_len;
  uint8_t prevNote;
  uint8_t seq[16];
  uint8_t index;

public:
  uint8_t octave;
  NoteFunction(uint8_t channel, uint8_t every, uint8_t octave)
  {
    this->channel = channel;
    this->every = every;
    this->note = 0;
    // this->note_len = 1;
    this->prevNote = 0;
    for (uint8_t i = 0; i < 16; i++) {
      this->seq[i] = 1;
    }
    this->octave = octave;
  }
  void toggleOptionMenu()
  {
    for (uint8_t i = 0; i < 16; i++) {
      setTileColor(i, this->seq[i] == 0 ? 0x000000 : blue);
    }
  }
  void setIndexPlaying()
  {
    setTileColor(this->index, green);
    uint8_t key = this->index > 0 ? this->index - 1 : 15;
    setTileColor(key, this->seq[key] == 0 ? 0x000000 : blue);
  }
  void pulse(AppState* appState)
  {
    if (appState->pulses % this->every == 0) {
      // this->note_len--;
      if (!appState->optionMenu) {
        setIndexPlaying();
      }
      if (this->seq[this->index] == 1 /*&& this->note_len == 0*/) {
        this->note = getRandomNote();
        // this->note_len = lengths[getRandomNumber(0, 5)];
        // Serial.println(note_len);
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
  void toggleKey(uint8_t key)
  {
    Serial.println(key);
    uint8_t value = this->seq[key];
    this->seq[key] = value == 0 ? 1 : 0;
    setTileColor(key, value == 0 ? blue : 0x000000);
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
    //    Serial.println(octave);
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
  void clean() { this->index = 0; }
};
#endif
