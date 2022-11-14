#ifndef Trellis_h
#define Trellis_h
#include "Adafruit_NeoTrellis.h"
Adafruit_NeoTrellis trellis;

uint32_t white = trellis.pixels.Color(255, 255, 255);
uint32_t green = trellis.pixels.Color(133, 255, 0);

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
