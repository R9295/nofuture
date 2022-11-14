#ifndef Trellis_h
#define Trellis_h
#include "Adafruit_NeoTrellis.h"
Adafruit_NeoTrellis trellis;

uint32_t blue = 0x000033;
uint32_t green = trellis.pixels.Color(133, 255, 0);
void
setTileColor(uint8_t tile, uint32_t color)
{
  trellis.pixels.setPixelColor(tile, color);
  trellis.pixels.show();
}
void
clearTile(uint16_t tileId)
{
  trellis.pixels.setPixelColor(tileId, 0x000000);
  trellis.pixels.show();
}

void
clearBoard()
{
  for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++) {
    clearTile(i);
  }
}

uint32_t
Wheel(byte WheelPos)
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
{
  if (WheelPos < 85) {
    return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}
uint8_t
getIndexFromKey(uint8_t value)
{
  uint8_t index = 0;
  switch (value) {
    case 3:
      index = 0;
      break;
    case 7:
      index = 1;
      break;
    case 11:
      index = 2;
      break;
    case 15:
      index = 3;
      break;
    case 19:
      index = 4;
      break;
    case 23:
      index = 5;
      break;
    case 27:
      index = 6;
      break;
    case 31:
      index = 7;
      break;
    case 35:
      index = 8;
      break;
    case 39:
      index = 9;
      break;
    case 43:
      index = 10;
      break;
    case 47:
      index = 11;
      break;
    case 51:
      index = 12;
      break;
    case 55:
      index = 13;
      break;
    case 59:
      index = 14;
      break;
    case 63:
      index = 15;
      break;
  }
  return index;
}

void
initTrellisAnimation()
{
  // do a little animation to show we're on
  for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(
      i, Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(100);
  }
}
#endif
