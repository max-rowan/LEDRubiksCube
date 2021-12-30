#include <DisplayUtil.h>

uint16_t DisplayUtil::XY(uint8_t x, uint8_t y) {
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

CRGB DisplayUtil::_getLED(CRGB leds[NUM_LEDS], int i) {
  if (i < 0 || i >= NUM_LEDS)
    return NULL;
  return leds[i];
}

CRGB *DisplayUtil::_getPLED(CRGB leds[NUM_LEDS], int i) {
  if (i < 0 || i >= NUM_LEDS)
    return NULL;
  return &leds[i];
}

/**
 * Sets the color of an LED in the LED Matrix
 */
void DisplayUtil::setMatrixLED(int yStart, int x, int y, CRGB::HTMLColorCode color,
                               CRGB leds[NUM_LEDS]) {

  // Return if any coord is out of bounds
  if (x < 0 || x > 7 || y < 0 || y > 7)
    return;

  int xy = XY(x, yStart + y);

  if (xy < 0 || xy >= NUM_LEDS)
    return;

  leds[xy] = color;
}

/**
 * Gets a pointer to an LED in the LED Matrix
 */
CRGB *DisplayUtil::getMatrixLED(int yStart, int x, int y, CRGB leds[NUM_LEDS]) {

  // Return if any coord is out of bounds
  if (x < 0 || x > 7 || y < 0 || y > 7)
    return NULL;

  int xy = XY(x, yStart + y);

  if (xy < 0 || xy >= NUM_LEDS)
    return NULL;

  return &leds[xy];
}

/**
 * Maps a single x, y coordinate pairing to four LED matrix coordinates
 */
void DisplayUtil::mapCoordToLEDs(int faceNum, int x, int y, CRGB::HTMLColorCode color,
                                 CRGB leds[NUM_LEDS]) {
  int yStart = faceNum * 8;
  setMatrixLED(yStart, x, y, color, leds);
  setMatrixLED(yStart, x + 1, y, color, leds);
  setMatrixLED(yStart, x, y + 1, color, leds);
  setMatrixLED(yStart, x + 1, y + 1, color, leds);
}

/**
 * Maps a CubePanel to four LED matrix coordinates
 */
void DisplayUtil::mapPanelToLEDs(CubePanel panel, CRGB leds[NUM_LEDS]) {
  int x = 3 * panel.x;
  int y = 3 * panel.y;
  mapCoordToLEDs(panel.faceNum, x, y, panel.color, leds);
}

/**
 * Maps a row/col of CubePanels on to LED matrix coords
 */
void DisplayUtil::mapPanelsToLEDs(CubePanel panels[3], CRGB leds[NUM_LEDS]) {
  for (size_t i = 0; i < 3; i++) {
    mapPanelToLEDs(panels[i], leds);
  }
}

/**
 * Maps a row/col of CubePanels on to LED matrix coords
 */
void DisplayUtil::mapPanelsToLEDs(CubePanel *panels[3], CRGB leds[NUM_LEDS]) {
  for (size_t i = 0; i < 3; i++) {
    mapPanelToLEDs(*panels[i], leds);
  }
}

/**
 * Maps all CubePanels on a CubeFace to LED matrix coords
 */
void DisplayUtil::mapFaceToLEDs(CubeFace face, CRGB leds[NUM_LEDS]) {
  for (size_t y = 0; y < 3; y++) {
    for (size_t x = 0; x < 3; x++) {
      CubePanel panel = face.face[y][x];
      mapPanelToLEDs(panel, leds);
    }
  }
}

/**
 * Gets the LEDs that correspond to the panel's coords
 * @param panel - panel obj in question
 * @param leds - full array of leds
 * @param panelLEDs - pointers to the leds that map to the panel
 */
void DisplayUtil::getPanelLEDs(CubePanel panel, CRGB leds[NUM_LEDS], CRGB *panelLEDs[3][3]) {
  int x0 = panel.x;
  int y0 = panel.faceNum * 8 + panel.y;

  for (int i = 0; i < 3; i++) {
    int y = y0 + i;

    for (int j = 0; j < 3; j++) {
      int x = x0 + j;

      panelLEDs[i][j] = &leds[XY(x, y)];
    }
  }
}

/**
 * Clears a given row of a CubeFace (0-2)
 */
void DisplayUtil::clearFaceRow(int faceNum, int rowNum, CRGB leds[NUM_LEDS]) {
  int matrixRowNum = rowNum * 3;
  clearMatrixRow(faceNum, matrixRowNum, leds);
  clearMatrixRow(faceNum, matrixRowNum + 1, leds);
}

/**
 * Clears a given col of a CubeFace (0-2)
 */
void DisplayUtil::clearFaceCol(int faceNum, int colNum, CRGB leds[NUM_LEDS]) {
  int matrixColNum = colNum * 3;
  clearMatrixCol(faceNum, matrixColNum, leds);
  clearMatrixCol(faceNum, matrixColNum + 1, leds);
}

/**
 * Gets a given row of a matrix (0-7)
 */
void DisplayUtil::getMatrixRow(int faceNum, int matrixRowNum, CRGB leds[NUM_LEDS],
                               CRGB *rowLeds[matrixWidth]) {
  int yStart = faceNum * 8;
  for (int x = 0; x < 8; x++) {
    CRGB *pLed = getMatrixLED(yStart, x, matrixRowNum, leds);
    rowLeds[x] = pLed;
  }
}

/**
 * Gets a given col of a matrix (0-7)
 */
void DisplayUtil::getMatrixCol(int faceNum, int matrixColNum, CRGB leds[NUM_LEDS],
                               CRGB *colLeds[matrixWidth]) {
  int yStart = faceNum * 8;
  for (int i = 0; i < 8; i++) {
    CRGB *pLed = getMatrixLED(yStart, matrixColNum, i, leds);
    colLeds[i] = pLed;
  }
}

/**
 * Clears a given row of a matrix (0-7)
 */
void DisplayUtil::clearMatrixRow(int faceNum, int matrixRowNum, CRGB leds[NUM_LEDS]) {
  int yStart = faceNum * 8;
  for (int x = 0; x < 8; x++) {
    setMatrixLED(yStart, x, matrixRowNum, CRGB::Black, leds);
  }
}

/**
 * Clears a given col of a matrix (0-7)
 */
void DisplayUtil::clearMatrixCol(int faceNum, int matrixColNum, CRGB leds[NUM_LEDS]) {
  int yStart = faceNum * 8;
  for (int y = 0; y < 8; y++) {
    setMatrixLED(yStart, matrixColNum, y, CRGB::Black, leds);
  }
}
