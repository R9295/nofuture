#ifndef AppState_h
#define AppState_h
#include "midi.h"
#include "trellis.h"
class AppState
{
public:
  long bars;
  uint8_t pulses;
  bool optionMenu;
  bool keysPressed[16];
  AppState()
  {
    this->bars = 1;
    this->pulses = 0;
    this->optionMenu = false;
    for (uint8_t i = 0; i < 16; i++) {
      keysPressed[i] = false;
    }
  }
  void pulse()
  {
    this->pulses++;
    if (this->pulses == PULSES_IN_BAR) {
      this->bars++;
      this->pulses = 0;
    }
  }
  void clean()
  {
    this->pulses = 0;
    this->bars = 1;
  }
  void toggleOptionMenu() {
    this->optionMenu = !this->optionMenu;
    if (this->optionMenu == true) {
      setTileColorRanged(0, 7, 5184848);
      setTileColorRanged(8, 15, 13269299);
    }
  }
};
#endif
