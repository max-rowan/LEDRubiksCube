#include <FastLED.h>

/**
 * Represents a single Panel (single square color).
 * Coordinates are local to the parent face
 */
struct CubePanel {
  uint8_t faceNum;
  CRGB::HTMLColorCode color;
  CRGB::HTMLColorCode panelColor;
  uint8_t x;
  uint8_t y;
  uint8_t z;

  CubePanel( );
  CubePanel(CRGB::HTMLColorCode);
  CubePanel(CRGB::HTMLColorCode, uint8_t, uint8_t, uint8_t);

  boolean isCorrectColor( );
};