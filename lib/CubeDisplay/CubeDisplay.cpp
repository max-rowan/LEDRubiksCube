#include <CubeDisplay.h>

CoordSet::CoordSet(double x, double y) : x(x), y(y) { }

RubiksCube *CubeDisplay::pCube = NULL;
CRGB CubeDisplay::leds[NUM_LEDS] = {CRGB::Black};

void CubeDisplay::init( ) { setCubeLEDs( ); }

uint16_t CubeDisplay::_XY(uint8_t x, uint8_t y) {
  uint16_t i;

  if (y & 0x01) {
    // Odd rows run forwards
    i = (y * matrixWidth) + x;
  } else {
    // Even rows run backwards
    uint8_t reverseX = (matrixWidth - 1) - x;
    i = (y * matrixWidth) + reverseX;
  }

  return i;
}

/**************************************************************************************************/
/**************************************** Edge Animation ******************************************/
bool CubeDisplay::_isEdgeRow(CubePanel *edge[3]) { return edge[0]->y == edge[1]->y; }

int CubeDisplay::_getEdgeDir(CubePanel *edge[3]) {
  if (_isEdgeRow(edge)) {
    return edge[0]->x < edge[1]->x ? 1 : -1;
  } else {
    return edge[0]->y < edge[1]->y ? 1 : -1;
  }
}

void CubeDisplay::_clearCubeFaceEdges(CubeFace *cf) {
  _clearEdgeLEDs(cf->upperBorderEdge);
  _clearEdgeLEDs(cf->rightBorderEdge);
  _clearEdgeLEDs(cf->bottomBorderEdge);
  _clearEdgeLEDs(cf->leftBorderEdge);
}

void CubeDisplay::_clearFaceRow(int faceNum, int rowNum) {
  int matrixRowNum = rowNum * 3;
  _clearMatrixRow(faceNum, matrixRowNum);
  _clearMatrixRow(faceNum, matrixRowNum + 1);
}

void CubeDisplay::_clearFaceCol(int faceNum, int colNum) {
  int matrixColNum = colNum * 3;
  _clearMatrixCol(faceNum, matrixColNum);
  _clearMatrixCol(faceNum, matrixColNum + 1);
}

void CubeDisplay::_clearMatrixRow(int faceNum, int matrixRowNum) {
  int yStart = faceNum * 8;
  for (int x = 0; x < 8; x++) {
    _setMatrixLED(yStart, x, matrixRowNum, CRGB::Black);
  }
}

void CubeDisplay::_clearMatrixCol(int faceNum, int matrixColNum) {
  int yStart = faceNum * 8;
  for (int y = 0; y < 8; y++) {
    _setMatrixLED(yStart, matrixColNum, y, CRGB::Black);
  }
}

void CubeDisplay::_clearFaceLEDs(CubeFace cf) {
  int start = cf.faceNum * 64;
  int end = start + 64;
  for (int i = start; i < end; i++) {
    leds[i] = CRGB::Black;
  }
}

void CubeDisplay::_clearEdgeLEDs(CubePanel *edge[3]) {
  int faceNum = edge[0]->faceNum;
  bool isRow = edge[0]->y == edge[1]->y;

  if (isRow) {
    int rowNum = edge[0]->y;
    _clearFaceRow(faceNum, rowNum);
  } else {
    int colNum = edge[0]->x;
    _clearFaceCol(faceNum, colNum);
  }
}

void CubeDisplay::_shiftRowEdge(int faceNum, int x, int y, int edgeDir, int shiftNum,
                                CubePanel *toEdge[3], CRGB::HTMLColorCode color) {
  x = x + shiftNum * edgeDir;
  if (x < 7 && x >= 0) {
    _mapCoordToLEDs(faceNum, x, y, color);
  } else if (x > 7 || x < -1) {
    shiftNum = x - 8 * edgeDir;
    _shiftToEdge(toEdge, shiftNum, color);
  } else {
    _mapCoordToLEDs(faceNum, x, y, color);

    shiftNum = x - 8 * edgeDir;
    _shiftToEdge(toEdge, shiftNum, color);
  }
}

void CubeDisplay::_shiftColEdge(int faceNum, int x, int y, int edgeDir, int shiftNum,
                                CubePanel *toEdge[3], CRGB::HTMLColorCode color) {
  y = y + shiftNum * edgeDir;
  if (y < 7 && y >= 0) {
    _mapCoordToLEDs(faceNum, x, y, color);
  } else if (y > 7 || y < -1) {
    shiftNum = y - 8 * edgeDir;
    _shiftToEdge(toEdge, shiftNum, color);
  } else {
    _mapCoordToLEDs(faceNum, x, y, color);

    shiftNum = y - 8 * edgeDir;
    _shiftToEdge(toEdge, shiftNum, color);
  }
}

void CubeDisplay::_shiftToEdge(CubePanel *edge[3], int shiftNum, CRGB::HTMLColorCode color) {
  int faceNum = edge[0]->faceNum;
  bool isRow = _isEdgeRow(edge);
  int edgeDir = _getEdgeDir(edge);

  int x = edge[0]->x * 3;
  int y = edge[0]->y * 3;

  if (isRow) {
    _shiftEdgeToRow(faceNum, edgeDir, shiftNum, x, y, color);
  } else {
    _shiftEdgeToCol(faceNum, edgeDir, shiftNum, x, y, color);
  }
}

/**
 * Shifts a color some shiftNum number of units along an edge that is a row
 *
 * @param faceNum   Face Number
 * @param edgeDir   direction the edge is facing (1 if the row goes from 0->7, -1 if the row goes
 * from 7->0)
 * @param shiftNum  number of leds to shift (positive if shifting from the left, negative if
 * shifting from the right)
 * @param x        x-value of the led at the edge's 0th index
 * @param y         y-value of the led edge row
 * @param color     color to set the leds
 */
void CubeDisplay::_shiftEdgeToRow(int faceNum, int edgeDir, int shiftNum, int x, int y,
                                  CRGB::HTMLColorCode color) {

  x = x + shiftNum * edgeDir;
  _mapCoordToLEDs(faceNum, x, y, color);
}

/**
 * Shifts a color some shiftNum number of units along an edge that is a column
 *
 * @param faceNum   Face Number
 * @param edgeDir   direction the edge is facing (1 if the row goes from 0->7, -1 if the row goes
 * from 7->0)
 * @param shiftNum  number of leds to shift (positive if shifting from the left, negative if
 * shifting from the right)
 * @param x         x-value of the led edge column
 * @param y         y-value of the led at the edge's 0th index
 * @param color     color to set the leds
 */
void CubeDisplay::_shiftEdgeToCol(int faceNum, int edgeDir, int x, int y, int shiftNum,
                                  CRGB::HTMLColorCode color) {

  y = y + shiftNum * edgeDir;
  _mapCoordToLEDs(faceNum, x, y, color);
}

void CubeDisplay::_shiftEdgePanel(int faceNum, int x, int y, int edgeDir, bool isEdgeRow,
                                  int shiftNum, CubePanel *toEdge[3], CRGB::HTMLColorCode color) {
  if (isEdgeRow) {
    _shiftRowEdge(faceNum, x, y, edgeDir, shiftNum, toEdge, color);
  } else {
    _shiftColEdge(faceNum, x, y, edgeDir, shiftNum, toEdge, color);
  }
}

void CubeDisplay::_setEdgeLEDs(CubePanel *fromEdge[3], CubePanel *toEdge[3], double phi) {
  int fromFaceNum = fromEdge[0]->faceNum;
  int fromEdgeDir = _getEdgeDir(fromEdge);
  bool isFromEdgeRow = _isEdgeRow(fromEdge);

  // get num LEDs to shift - total is matrixWidth and we'll use phi's % of pi/2
  int shiftNum = (int)(matrixWidth * phi / (PI / 2));

  // rotation direction matches phi

  for (int i = 0; i < 3; i++) {
    _shiftEdgePanel(fromFaceNum, fromEdge[i]->x, fromEdge[i]->y, fromEdgeDir, isFromEdgeRow,
                    shiftNum, toEdge, fromEdge[i]->color);
  }
}
/*************************************** End Edge Animation ***************************************/
/**************************************************************************************************/

/**************************************************************************************************/
/*************************************** Edge Animation 2 *****************************************/

bool CubeDisplay::_edgeDirMatches(CubePanel *edge[3]) {
  bool isRow = _isEdgeRow(edge);
  return (isRow && edge[0]->x < edge[1]->x || !isRow && edge[0]->y < edge[1]->y);
}

/**
 *  Gets an array of pointers to the led color array
 *
 * @param faceNum   Face number: 0-6
 * @param rowNum    Row number of the cube panel: 0-2
 * @param edgeLeds  Array of pointers to led array
 */
void CubeDisplay::_getFaceLEDRow(int faceNum, int rowNum, CRGB *edgeLeds[16]) {
  int yStart = faceNum * 8;
  int y0 = rowNum * 3;

  for (int x = 0; x < 8; x++) {
    int y = yStart + y0;

    // Set pointer for the first row
    int idx = x;
    int rowNum = y0;
    if (y0 == 0 || y0 == 7) {
      idx = x + 8;
    }
    int xy = _XY(x, y);
    edgeLeds[idx] = &leds[xy];

    // Set pointer for the second row
    rowNum = y0 + 1;
    if (rowNum == 0 || rowNum == 7) {
      idx = x + 8;
    } else {
      idx = x;
    }
    xy = _XY(x, y + 1);
    edgeLeds[idx] = &leds[xy];
  }
}

void CubeDisplay::_getFaceLEDCol(int faceNum, int colNum, CRGB *edgeLeds[16]) {
  int yStart = faceNum * 8;
  int x = colNum * 3;

  for (int i = 0; i < 8; i++) {
    int y = yStart + i;

    // Set pointer for the first column
    int idx = i;
    int colNum = x;
    if (colNum == 0 || colNum == 7) {
      idx = i + 8;
    }
    int xy = _XY(colNum, y);
    edgeLeds[idx] = &leds[xy];

    // Set pointer for the second column
    idx = i;
    colNum = x + 1;
    if (colNum == 0 || colNum == 7) {
      idx = i + 8;
    }
    xy = _XY(colNum, y);
    edgeLeds[idx] = &leds[xy];
  }
}

void CubeDisplay::_invert(CRGB *edgeLEDs[16]) {
  CRGB *tempLED;
  for (int i = 0; i < 8; i++) {
    tempLED = edgeLEDs[i];
    edgeLEDs[i] = edgeLEDs[7 - i];
    edgeLEDs[8 - i] = tempLED;

    edgeLEDs[8 + i] = edgeLEDs[15 - i];
    edgeLEDs[15 - i] = tempLED;
  }
}

void CubeDisplay::_getLEDEdgePointers(CubePanel *edge[3], CRGB *edgeLeds[16]) {
  bool isRow = _isEdgeRow(edge);
  bool dirMatches = _edgeDirMatches(edge);
  int faceNum = edge[0]->faceNum;

  if (isRow) {
    int rowNum = edge[0]->y;
    _getFaceLEDRow(faceNum, rowNum, edgeLeds);
  } else {
    int colNum = edge[0]->x;
    _getFaceLEDCol(faceNum, colNum, edgeLeds);
  }

  if (!dirMatches) {
    _invert(edgeLeds);
  }
}

void CubeDisplay::_appendFaceEdgeLeds(CRGB *faceEdgeLeds[64], CRGB *edgeLeds[16], int startPos) {
  for (int i = 0; i < 8; i++) {
    faceEdgeLeds[startPos + i] = edgeLeds[i];
    faceEdgeLeds[32 + startPos + i] = edgeLeds[8 + i];
  }
}

void CubeDisplay::_getLEDFaceEdgePointers(CubeFace *face, CRGB *faceEdgeLeds[64]) {
  CRGB *edgeLeds[16];
  _getLEDEdgePointers(face->upperBorderEdge, edgeLeds);
  _appendFaceEdgeLeds(faceEdgeLeds, edgeLeds, 0);

  // _getLEDEdgePointers(face->rightBorderEdge, edgeLeds);
  // _appendFaceEdgeLeds(faceEdgeLeds, edgeLeds, 16);

  // _getLEDEdgePointers(face->bottomBorderEdge, edgeLeds);
  // _appendFaceEdgeLeds(faceEdgeLeds, edgeLeds, 32);

  // _getLEDEdgePointers(face->leftBorderEdge, edgeLeds);
  // _appendFaceEdgeLeds(faceEdgeLeds, edgeLeds, 48);
}

void CubeDisplay::_shiftLedsRight(CRGB *leds[64]) {
  CRGB *temp = leds[63];
  for (int i = 63; i >= 0; i--) {
    *leds[i + 1] = *leds[i];
  }
  *leds[0] = *temp;
}

void CubeDisplay::_shiftLedsLeft(CRGB *leds[64]) {
  CRGB *temp = leds[0];
  for (int i = 0; i < 63; i++) {
    *leds[i] = *leds[i + 1];
  }
  *leds[63] = *temp;
}

void CubeDisplay::_shiftLeds(CRGB *leds[64], int shiftNum) {
  if (shiftNum > 0) {
    _shiftLedsRight(leds);
  }
  if (shiftNum < 0) {
    _shiftLedsLeft(leds);
  }
}

/*************************************** End Edge Animation ***************************************/
/**************************************************************************************************/

void CubeDisplay::_setEdgesLEDs(CubeFace *cf, double phi) {
  // clear all edge LEDs
  _clearCubeFaceEdges(cf);

  // set edge leds
  CRGB *faceEdgeLeds[64];
  _getLEDFaceEdgePointers(cf, faceEdgeLeds);

  for (int i = 0; i < 16; i++) {
    *faceEdgeLeds[i] = CRGB::Turquoise;
  }

  // for (CRGB *led : faceEdgeLeds) {
  //   // Iterate over each edge, appending the row/col that's closer to the edge to 0-31,
  //   //  & the row/col that's closer to the center to 32-63

  //   //
  //   *led = CRGB::Turquoise;
  // }

  // int shiftNum = (int)(matrixWidth * phi / (PI / 2));
  // _shiftLeds(faceEdgeLeds, shiftNum);
}

// void CubeDisplay::_setEdgesLEDs(CubeFace *cf, double phi) {
//   // clear all edge LEDs
//   _clearCubeFaceEdges(cf);

//   // for each edge
//   _setEdgeLEDs(cf->upperBorderEdge, phi > 0 ? cf->leftBorderEdge : cf->rightBorderEdge, phi);
//   _setEdgeLEDs(cf->rightBorderEdge, phi > 0 ? cf->upperBorderEdge : cf->bottomBorderEdge, phi);
//   _setEdgeLEDs(cf->bottomBorderEdge, phi > 0 ? cf->rightBorderEdge : cf->leftBorderEdge, phi);
//   _setEdgeLEDs(cf->leftBorderEdge, phi > 0 ? cf->bottomBorderEdge : cf->upperBorderEdge, phi);
// }

void CubeDisplay::_setFaceLEDs(CubeFace *cf, double phi) {
  // turn off face LEDs
  _clearFaceLEDs(*cf);

  // set face's colors
  int yStart = cf->faceNum * 8;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      CRGB color = cf->face[j][i].color;

      // map to coordinate plane
      double x0 = i * 3.0;
      double y0 = 7.0 - (j * 3.0);
      CoordSet points[4] = {CoordSet(x0, y0), CoordSet(x0 + 1, y0), CoordSet(x0, y0 - 1),
                            CoordSet(x0 + 1, y0 - 1)};

      for (int p = 0; p < 4; p++) {
        // translate points to new origin
        points[p].x -= 3.5;
        points[p].y -= 3.5;

        // rotate points
        x0 = points[p].x;
        y0 = points[p].y;
        points[p].x = x0 * cos(phi) - y0 * sin(phi);
        points[p].y = y0 * cos(phi) + x0 * sin(phi);

        // translate points back
        points[p].x += 3.5;
        points[p].y += 3.5;

        // map to array position (y needs to be re-inverted for arr index)
        uint8_t x = (uint8_t)points[p].x;
        uint8_t y = 7 - (uint8_t)(points[p].y);
        if (x > 7 || y > 7)
          continue;

        uint16_t xy = _XY(x, yStart + y);
        leds[xy] = color;
      }
    }
  }
}

void CubeDisplay::_setMatrixLED(int yStart, int x, int y, CRGB::HTMLColorCode color) {
  if (x < 0 || x > 7 || y < 0 || y > 7)
    return;
  int xy = _XY(x, yStart + y);
  if (xy < 0 || xy >= NUM_LEDS)
    return;
  leds[xy] = color;
}

/**
 * Maps a single x, y coordinate pairing to four LED matrix coordinates
 */
void CubeDisplay::_mapCoordToLEDs(int faceNum, int x, int y, CRGB::HTMLColorCode color) {
  int yStart = faceNum * 8;
  _setMatrixLED(yStart, x, y, color);
  _setMatrixLED(yStart, x + 1, y, color);
  _setMatrixLED(yStart, x, y + 1, color);
  _setMatrixLED(yStart, x + 1, y + 1, color);
}

/**
 * Maps a CubePanel to four LED matrix coordinates
 */
void CubeDisplay::_mapPanelToLEDs(CubePanel panel) {
  int x = 3 * panel.x;
  int y = 3 * panel.y;
  _mapCoordToLEDs(panel.faceNum, x, y, panel.color);
}

void CubeDisplay::_mapFaceToLEDs(CubeFace face) {
  for (size_t y = 0; y < 3; y++) {
    for (size_t x = 0; x < 3; x++) {
      CubePanel panel = face.face[y][x];
      _mapPanelToLEDs(panel);
    }
  }
}

void CubeDisplay::setCubeLEDs(CubeFace *cf, double phi) {
  // setCubeLEDs( );
  _setEdgesLEDs(cf, phi);
  // _setFaceLEDs(cf, phi);
}

// void CubeDisplay::setCubeLEDs( ) {
//   CubeFace *faces[5] = {pCube->pFront, pCube->pRight, pCube->pBack, pCube->pLeft, pCube->pUpper};
//   for (CubeFace *face : faces) {
//     _setFaceLEDs(face, 0);
//   }
// }

void CubeDisplay::setCubeLEDs( ) {
  CubeFace faces[5] = {*pCube->pFront, *pCube->pRight, *pCube->pBack, *pCube->pLeft,
                       *pCube->pUpper};
  for (CubeFace face : faces) {
    _clearFaceLEDs(face);
    _mapFaceToLEDs(face);
  }
  // _clearFaceLEDs(*pCube->pFront);
  // _mapFaceToLEDs(*pCube->pFront);

  // _clearFaceLEDs(*pCube->pRight);
  // _mapFaceToLEDs(*pCube->pRight);

  // _clearFaceLEDs(*pCube->pBack);
  // _mapFaceToLEDs(*pCube->pBack);

  // _clearFaceLEDs(*pCube->pLeft);
  // _mapFaceToLEDs(*pCube->pLeft);

  // _clearFaceLEDs(*pCube->pUpper);
  // _mapFaceToLEDs(*pCube->pUpper);
}

void CubeDisplay::rotate(CubeFace *cf, boolean isInverse, int rotationMillis) {
  long t = millis( );
  int dt = 0;
  int direction = isInverse ? 1 : -1;

  setCubeLEDs( );

  while (dt < rotationMillis) {
    double phi = PI / 2 * dt / rotationMillis * direction;
    setCubeLEDs(cf, phi);
    dt = millis( ) - t;
    FastLED.show( );
  }
}

void CubeDisplay::display( ) {
  delay(50);
  setCubeLEDs( );
  FastLED.show( );
}
