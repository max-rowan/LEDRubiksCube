#include <EdgeController.h>

CRGB *EdgeController::pEdgeOuterLEDs[32] = {NULL};
CRGB *EdgeController::pEdgeInnerLEDs[32] = {NULL};

/**
 * Determines if an edge is a row or column
 */
bool EdgeController::_isEdgeRow(CubePanel *edge[3]) { return edge[0]->y == edge[1]->y; }

bool EdgeController::_edgeDirMatches(CubePanel *edge[3]) {
  bool isRow = _isEdgeRow(edge);
  return ((isRow && edge[0]->x < edge[1]->x) || (!isRow && edge[0]->y < edge[1]->y));
}

/**
 * Sets the EdgeController's edge led pointer arrays for a given row on a face
 * @param faceNum - num corresponding to the cube face we're getting the row from
 * @param rowNum - num of the row we're retrieving on the face
 * @param edgeStartIdx - num in the EdgeController's pointer arr we're starting at
 * @param dirMatches - bool if the direction of the arr matches the direction of the edge array
 * @param leds - array we're getting the colors from
 */
void EdgeController::_setFaceLEDRow(int faceNum, int rowNum, int edgeStartIdx, bool dirMatches,
                                    CRGB leds[NUM_LEDS]) {

  if (pEdgeOuterLEDs == NULL || pEdgeInnerLEDs == NULL)
    return;

  int yStart = faceNum * 8;
  int y0 = rowNum * 3;

  CRGB *topRow[8];
  CRGB *bottomRow[8];
  _du.getMatrixRow(faceNum, y0, leds, topRow);
  _du.getMatrixRow(faceNum, y0 + 1, leds, bottomRow);

  for (int x = 0; x < 8; x++) {
    int idx = edgeStartIdx + x;
    //   if (y0 == 0 || y0 == 7) {
    //     pEdgeOuterLEDs[idx] = topLED;
    //     pEdgeInnerLEDs[idx] = bottomLED;
    //   } else if (y0 == 1 || y0 == 6) {
    //     pEdgeOuterLEDs[idx] = bottomLED;
    //     pEdgeInnerLEDs[idx] = topLED;
    //   }
    int mappingIdx = dirMatches ? x : 7 - x;
    pEdgeOuterLEDs[idx] = topRow[mappingIdx];
    pEdgeInnerLEDs[idx] = bottomRow[mappingIdx];
  }
}

void EdgeController::_setFaceLEDCol(int faceNum, int colNum, int edgeStartIdx, bool dirMatches,
                                    CRGB leds[NUM_LEDS]) {

  if (pEdgeOuterLEDs == NULL || pEdgeInnerLEDs == NULL)
    return;

  int yStart = faceNum * 8;
  int x = colNum * 3;

  CRGB *leftRow[8];
  CRGB *rightRow[8];
  _du.getMatrixCol(faceNum, x, leds, leftRow);
  _du.getMatrixCol(faceNum, x + 1, leds, rightRow);

  for (int i = 0; i < 8; i++) {
    int idx = edgeStartIdx + i;
    //   if (x == 0 || x == 7) {
    //     pEdgeOuterLEDs[idx] = leftLED;
    //     pEdgeInnerLEDs[idx] = rightLED;
    //   } else if (x == 1 || x == 6) {
    //     pEdgeOuterLEDs[idx] = rightLED;
    //     pEdgeInnerLEDs[idx] = leftLED;
    //   }
    int mappingIdx = dirMatches ? i : 7 - i;

    pEdgeOuterLEDs[idx] = leftRow[mappingIdx];
    pEdgeInnerLEDs[idx] = rightRow[mappingIdx];
  }
}

void EdgeController::_invert(CRGB *edgeLEDs[16]) {
  CRGB *tempLED;
  for (int i = 0; i < 8; i++) {
    tempLED = edgeLEDs[i];
    edgeLEDs[i] = edgeLEDs[7 - i];
    edgeLEDs[8 - i] = tempLED;

    edgeLEDs[8 + i] = edgeLEDs[15 - i];
    edgeLEDs[15 - i] = tempLED;
  }
}

void EdgeController::_rotateEdges(double phi) {
  double percent = abs(phi / (PI / 2.0));
  int shiftNum = (percent * 8);

  for (int i = 0; i < shiftNum; i++) {
    if (phi > 0) {
      _rotateLeft( );
    } else if (phi < 0) {
      _rotateRight( );
    }
  }
}

void EdgeController::_rotateRight( ) {
  CRGB led = *pEdgeOuterLEDs[31];
  for (int i = 31; i > 0; i--) {
    *pEdgeOuterLEDs[i] = *pEdgeOuterLEDs[i - 1];
    *pEdgeInnerLEDs[i] = *pEdgeInnerLEDs[i - 1];
  }

  *pEdgeOuterLEDs[0] = led;
  *pEdgeInnerLEDs[0] = led;
}

void EdgeController::_rotateLeft( ) {
  CRGB led = *pEdgeOuterLEDs[0];
  for (int i = 0; i < 31; i++) {
    *pEdgeOuterLEDs[i] = *pEdgeOuterLEDs[i + 1];
    *pEdgeInnerLEDs[i] = *pEdgeInnerLEDs[i + 1];
  }

  *pEdgeOuterLEDs[31] = led;
  *pEdgeInnerLEDs[31] = led;
}

void EdgeController::_setLEDEdgePointers(CubePanel *edge[3], CRGB leds[NUM_LEDS], int startIdx) {
  bool isRow = _isEdgeRow(edge);
  bool dirMatches = _edgeDirMatches(edge);
  int faceNum = edge[0]->faceNum;

  if (isRow) {
    int rowNum = edge[0]->y;
    _setFaceLEDRow(faceNum, rowNum, startIdx, dirMatches, leds);
  } else {
    int colNum = edge[0]->x;
    _setFaceLEDCol(faceNum, colNum, startIdx, dirMatches, leds);
  }
}

void EdgeController::_clearCubeFaceEdges(CubeFace *cf, CRGB leds[NUM_LEDS]) {
  _clearEdgeLEDs(cf->upperBorderEdge, leds);
  _clearEdgeLEDs(cf->rightBorderEdge, leds);
  _clearEdgeLEDs(cf->bottomBorderEdge, leds);
  _clearEdgeLEDs(cf->leftBorderEdge, leds);
}

void EdgeController::_clearEdgeLEDs(CubePanel *edge[3], CRGB leds[NUM_LEDS]) {
  int faceNum = edge[0]->faceNum;
  bool isRow = edge[0]->y == edge[1]->y;

  if (isRow) {
    int rowNum = edge[0]->y;
    _du.clearFaceRow(faceNum, rowNum, leds);
  } else {
    int colNum = edge[0]->x;
    _du.clearFaceCol(faceNum, colNum, leds);
  }
}

void EdgeController::_resetEdgeLEDs(CubeFace *cf, CRGB leds[NUM_LEDS]) {
  _du.mapPanelsToLEDs(cf->upperBorderEdge, leds);
  _du.mapPanelsToLEDs(cf->rightBorderEdge, leds);
  _du.mapPanelsToLEDs(cf->bottomBorderEdge, leds);
  _du.mapPanelsToLEDs(cf->leftBorderEdge, leds);
}

/**
 *  Sets the LEDs for the panels adjacent to a Face
 */
void EdgeController::_setCubeFaceEdges(CubeFace *cf, CRGB leds[NUM_LEDS]) {
  // Get the adjacent LEDs in clockwise order for the inner and outer edge rings
  _setLEDEdgePointers(cf->upperBorderEdge, leds, 0);
  _setLEDEdgePointers(cf->rightBorderEdge, leds, 8);
  _setLEDEdgePointers(cf->bottomBorderEdge, leds, 16);
  _setLEDEdgePointers(cf->leftBorderEdge, leds, 24);

  // *TEST - set the pointer arrays to be turquoise
  if (pEdgeInnerLEDs == NULL || pEdgeOuterLEDs == NULL)
    return;

  for (int i = 0; i < 32; i++) {
    if (pEdgeInnerLEDs[i] == NULL || pEdgeOuterLEDs[i] == NULL)
      continue;

    // *pEdgeInnerLEDs[i] = CRGB::Turquoise;
    // *pEdgeOuterLEDs[i] = CRGB::Turquoise;
  }
}

void EdgeController::setEdgesLEDs(CubeFace *cf, double phi, CRGB leds[NUM_LEDS]) {
  _clearCubeFaceEdges(cf, leds);
  _resetEdgeLEDs(cf, leds);
  _setCubeFaceEdges(cf, leds);
  _rotateEdges(phi);
}