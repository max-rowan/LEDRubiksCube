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
  static bool _isEdgeRow(CubePanel *[3]);
  static void _getLEDEdgePointers(CubePanel *[3], CRGB *[16]);
  static void _getLEDFaceEdgePointers(CubeFace *, CRGB *[64]);
  static int _getEdgeDir(CubePanel *[3]);
  static void _clearCubeFaceEdges(CubeFace *cf);
  static void _clearFaceRow(int, int);
  static void _clearFaceCol(int, int);
  static void _clearMatrixRow(int, int);
  static void _clearMatrixCol(int, int);
  static void _clearFaceLEDs(CubeFace *);
  static void _clearEdgeLEDs(CubePanel *[3]);
  static void _setEdgeLEDs(CubePanel *[3], CubePanel *[3], double);
  static void _setEdgesLEDs(CubeFace *, double);
  static void _shiftRowEdge(int, int, int, int, int, CubePanel *[3], CRGB::HTMLColorCode);
  static void _shiftColEdge(int, int, int, int, int, CubePanel *[3], CRGB::HTMLColorCode);
  static void _shiftToEdge(CubePanel *[3], int, CRGB::HTMLColorCode);
  static void _shiftEdgeToRow(int, int, int, int, int, CRGB::HTMLColorCode);
  static void _shiftEdgeToCol(int, int, int, int, int, CRGB::HTMLColorCode);
  static void _shiftEdgePanel(int, int, int, int, bool, int, CubePanel *[3], CRGB::HTMLColorCode);
  static void _setFaceLEDs(CubeFace *, double);
  static void _setMatrixLED(int, int, int, CRGB::HTMLColorCode);
  static void _mapCoordToLEDs(int, int, int, CRGB::HTMLColorCode);

public:
  static CRGB leds[NUM_LEDS];
  static RubiksCube *pCube;
  static void init( );
  static void setCubeLEDs(CubeFace *, double);
  static void setCubeLEDs( );
  static void rotate(CubeFace *, boolean, int);
  static void display( );
};
