#include <FastLED.h>

class CubeFace {
 private:
  void _rotateFaceCW();
  void _rotateFaceCCW();
  void _rotateEdgesCW();
  void _rotateEdgesCCW();
  void _rotateEdge(CRGB::HTMLColorCode**, CRGB::HTMLColorCode**);
  void _setBorderEdge(CRGB::HTMLColorCode**, CRGB::HTMLColorCode*,
                      CRGB::HTMLColorCode*, CRGB::HTMLColorCode*);

 public:
  CRGB::HTMLColorCode color;
  CRGB::HTMLColorCode face[3][3];

  CRGB::HTMLColorCode* upperBorderEdge[3];
  CRGB::HTMLColorCode* rightBorderEdge[3];
  CRGB::HTMLColorCode* bottomBorderEdge[3];
  CRGB::HTMLColorCode* leftBorderEdge[3];

  CubeFace();
  CubeFace(CRGB::HTMLColorCode);

  void rotateCW();
  void rotateCCW();

  void setFace(CRGB::HTMLColorCode[3][3]);

  void setUpperFaceBorders(CubeFace*, CubeFace*, CubeFace*, CubeFace*);
  void setBottomFaceBorders(CubeFace*, CubeFace*, CubeFace*, CubeFace*);
  void setSideFaceBorder(CubeFace*);
};