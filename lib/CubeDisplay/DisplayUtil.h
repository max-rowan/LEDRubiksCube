#ifndef DISPLAY_UTIL_H
#define DISPLAY_UTIL_H

#include <RubiksCube.h>

const int numFaces = 6;
const int matrixWidth = 8;
const int matrixHeight = 48;

#define NUM_LEDS (matrixWidth * matrixHeight)
#define DATA_PIN 5

struct CoordSet {
  double x;
  double y;
  CoordSet(double, double);
};

class DisplayUtil {
public:
  static uint16_t XY(uint8_t, uint8_t);

  static CRGB _getLED(CRGB leds[NUM_LEDS], int i);
  static CRGB *_getPLED(CRGB leds[NUM_LEDS], int i);
  static CRGB *getMatrixLED(int, int, int, CRGB[NUM_LEDS]);
  static void setMatrixLED(int, int, int, CRGB::HTMLColorCode, CRGB[NUM_LEDS]);
  static void mapCoordToLEDs(int, int, int, CRGB::HTMLColorCode, CRGB[NUM_LEDS]);
  static void mapPanelToLEDs(CubePanel, CRGB[NUM_LEDS]);
  static void mapPanelsToLEDs(CubePanel[3], CRGB[NUM_LEDS]);
  static void mapPanelsToLEDs(CubePanel *[3], CRGB[NUM_LEDS]);
  static void mapFaceToLEDs(CubeFace, CRGB[NUM_LEDS]);

  static void getPanelLEDs(CubePanel, CRGB[NUM_LEDS], CRGB *[3][3]);

  static void getMatrixRow(int, int, CRGB[NUM_LEDS], CRGB *[matrixWidth]);
  static void getMatrixCol(int, int, CRGB[NUM_LEDS], CRGB *[matrixWidth]);

  static void clearFaceRow(int, int, CRGB[NUM_LEDS]);
  static void clearFaceCol(int, int, CRGB[NUM_LEDS]);
  static void clearMatrixRow(int, int, CRGB[NUM_LEDS]);
  static void clearMatrixCol(int, int, CRGB[NUM_LEDS]);
};

#endif