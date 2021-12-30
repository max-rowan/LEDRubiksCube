#ifndef EDGE_CONTROLLER_H
#define EDGE_CONTROLLER_H

#include <DisplayUtil.h>
#include <RubiksCube.h>

struct EdgePanel {
  CRGB color;
  CRGB *led;
};

class EdgeController {
private:
  static DisplayUtil _du;

  static uint16_t _XY(uint8_t, uint8_t);
  static bool _isEdgeRow(CubePanel *[3]);
  static bool _edgeDirMatches(CubePanel *[3]);

  static void _setLEDEdgePointers(CubePanel *[3], CRGB leds[NUM_LEDS], int startIdx);
  static void _setLEDFaceEdgePointers(CubeFace *, CRGB leds[NUM_LEDS], int startIdx);

  static void _setFaceLEDRow(int faceNum, int rowNum, int startIdx, bool dirMatches,
                             CRGB leds[NUM_LEDS]);
  static void _setFaceLEDCol(int faceNum, int colNum, int startIdx, bool dirMatches,
                             CRGB leds[NUM_LEDS]);

  static void _invert(CRGB *[16]);

  static void _rotateEdges(double);
  static void _rotateRight();
  static void _rotateLeft();

  static void _clearCubeFaceEdges(CubeFace *cf, CRGB leds[NUM_LEDS]);
  static void _clearEdgeLEDs(CubePanel *[3], CRGB leds[NUM_LEDS]);
  static void _resetEdgeLEDs(CubeFace *, CRGB[NUM_LEDS]);

  static void _setCubeFaceEdges(CubeFace *, CRGB[NUM_LEDS]);

public:
  static CRGB *pEdgeOuterLEDs[32];
  static CRGB *pEdgeInnerLEDs[32];
  static void setEdgesLEDs(CubeFace *, double phi, CRGB leds[NUM_LEDS]);
};

#endif