#define FASTLED_INTERNAL
#include <RubiksCube.h>

const int matrixWidth = 8;
const int matrixHeight = 40;
#define NUM_LEDS (matrixWidth * matrixHeight)
#define DATA_PIN 5

struct CoordSet {
  double x;
  double y;
  CoordSet(double, double);
};

class CubeDisplay {
 private:
  static uint16_t _XY(uint8_t, uint8_t);
  static void _clearFaceLEDs(CubeFace *, int);
  static void _setEdgeLEDs(CubeFace *, int, double);
  static void _setFaceLEDs(CubeFace *, int, double);

 public:
  static CRGB leds[NUM_LEDS];
  static RubiksCube *pCube;
  static void init();
  static void setCubeLEDs(CubeFace *, double);
  static void setCubeLEDs();
  static void rotate(CubeFace *, boolean, int);
  static void display();
};
