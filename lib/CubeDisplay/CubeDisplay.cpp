#include <CubeDisplay.h>

CoordSet::CoordSet(double x, double y) : x(x), y(y) {}

RubiksCube* CubeDisplay::pCube = NULL;
CRGB CubeDisplay::leds[NUM_LEDS] = {CRGB::Black};

void CubeDisplay::init() { setCubeLEDs(); }

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

// uint16_t CubeDisplay::_XY(uint8_t x, uint8_t y) {
//   uint16_t i;

//   if (y & 0x01) {
//     // Odd rows run backwards
//     uint8_t reverseX = (matrixWidth - 1) - x;
//     i = (y * matrixWidth) + reverseX;
//   } else {
//     // Even rows run forwards
//     i = (y * matrixWidth) + x;
//   }

//   return i;
// }

void CubeDisplay::_clearFaceLEDs(CubeFace* cf, int yStart) {
  // int ledStart = _XY(0, yStart);
  // for (int i = ledStart; i < ledStart + 64; i++) {
  //   leds[i] = CRGB::Black;
  // }
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      int xy = _XY(x, yStart + y);
      leds[xy] = CRGB::Black;
    }
  }
}

void CubeDisplay::_setEdgeLEDs(CubeFace* cf, int yStart, double phi) {}

void CubeDisplay::_setFaceLEDs(CubeFace* cf, int yStart, double phi) {
  // turn off face LEDs
  _clearFaceLEDs(cf, yStart);

  // set face's colors
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      CRGB color = cf->face[j][i];

      // map to coordinate plane
      double x0 = i * 3.0;
      double y0 = 7.0 - (j * 3.0);
      CoordSet points[4] = {CoordSet(x0, y0), CoordSet(x0 + 1, y0),
                            CoordSet(x0, y0 - 1), CoordSet(x0 + 1, y0 - 1)};

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

        // map to nearest led coordinate

        // map to array position (y needs to be re-inverted for arr index)
        uint8_t x = (uint8_t)points[p].x;
        uint8_t y = 7 - (uint8_t)(points[p].y);
        if (x > 7 || y > 7) continue;

        uint16_t xy = _XY(x, yStart + y);
        leds[xy] = color;
      }
    }
  }
}

void CubeDisplay::setCubeLEDs(CubeFace* cf, double phi) {
  setCubeLEDs();
  CubeFace* faces[5] = {&pCube->front, &pCube->right, &pCube->back,
                        &pCube->left, &pCube->upper};

  int yStart = 0;
  for (CubeFace* face : faces) {
    if (cf == face) {
      _setFaceLEDs(face, yStart, phi);
      // _setEdgeLEDs(face, yStart, phi);
    };
    yStart += 8;
  }
}

void CubeDisplay::setCubeLEDs() {
  CubeFace* faces[5] = {&pCube->front, &pCube->right, &pCube->back,
                        &pCube->left, &pCube->upper};
  int yStart = 0;
  for (CubeFace* face : faces) {
    _setFaceLEDs(face, yStart, 0);
    yStart += 8;
  }
}

void CubeDisplay::rotate(CubeFace* cf, boolean isInverse, int rotationMillis) {
  long t = millis();
  int dt = 0;
  int direction = isInverse ? 1 : -1;

  setCubeLEDs();

  while (dt < rotationMillis) {
    double phi = PI / 2 * dt / rotationMillis * direction;
    setCubeLEDs(cf, phi);
    dt = millis() - t;
    FastLED.show();
  }
}

void CubeDisplay::display() {
  delay(50);
  setCubeLEDs();
  FastLED.show();
}
