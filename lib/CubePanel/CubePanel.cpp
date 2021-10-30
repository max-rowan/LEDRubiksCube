#include <CubePanel.h>

CubePanel::CubePanel( ) { };
CubePanel::CubePanel(CRGB::HTMLColorCode color) : color(color) { };
CubePanel::CubePanel(CRGB::HTMLColorCode color, uint8_t faceNum, uint8_t x, uint8_t y)
    : faceNum(faceNum), color(color), panelColor(color), x(x), y(y), z(0) { };

boolean CubePanel::isCorrectColor( ) { return color == panelColor; }