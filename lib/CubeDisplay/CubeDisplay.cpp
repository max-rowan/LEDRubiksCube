#include <CubeDisplay.h>

CoordSet::CoordSet(double x, double y) : x(x), y(y) { }

RubiksCube *CubeDisplay::pCube = NULL;
CRGB CubeDisplay::leds[NUM_LEDS] = {CRGB::Black};

void CubeDisplay::init( ) { setCubeLEDs( ); }
void CubeDisplay::_clearFaceLEDs(CubeFace cf) {
  int start = cf.faceNum * 64;
  int end = start + 64;
  for (int i = start; i < end; i++) {
    leds[i] = CRGB::Black;
  }
}

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

        uint16_t xy = _du.XY(x, yStart + y);
        leds[xy] = color;
      }
    }
  }
}

// void CubeDisplay::setCubeLEDs( ) {
//   CubeFace *faces[5] = {pCube->pFront, pCube->pRight, pCube->pBack, pCube->pLeft, pCube->pUpper};
//   for (CubeFace *face : faces) {
//     _setFaceLEDs(face, 0);
//   }
// }

void CubeDisplay::setCubeLEDs( ) {
  CubeFace faces[6] = {*pCube->pFront, *pCube->pRight, *pCube->pBack,
                       *pCube->pLeft,  *pCube->pUpper, *pCube->pBottom};
  for (CubeFace face : faces) {
    _clearFaceLEDs(face);
    _du.mapFaceToLEDs(face, leds);
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

void CubeDisplay::setCubeLEDs(CubeFace *cf, double phi) {
  setCubeLEDs( );
  _setFaceLEDs(cf, phi);
  _ec.setEdgesLEDs(cf, phi, leds);
}

void CubeDisplay::rotate(CubeFace *cf, boolean isInverse, int rotationMillis) {
  long t = millis( );
  int dt = 0;
  double direction = isInverse ? 1.0 : -1.0;

  setCubeLEDs( );

  while (dt < rotationMillis) {
    double phi = (PI / 2) * ((double)dt / rotationMillis) * direction;
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
