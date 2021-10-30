#include <CubePanel.h>
class CubeFace {
private:
  void _rotateFaceCW( );
  void _rotateFaceCCW( );
  void _rotateEdgesCW( );
  void _rotateEdgesCCW( );
  void _rotateEdge(CubePanel **, CubePanel **);
  void _setBorderEdge(CubePanel **, CubePanel *, CubePanel *, CubePanel *);

public:
  uint8_t faceNum;
  CRGB::HTMLColorCode color;
  CubePanel face[3][3];

  CubePanel *upperBorderEdge[3];
  CubePanel *rightBorderEdge[3];
  CubePanel *bottomBorderEdge[3];
  CubePanel *leftBorderEdge[3];

  CubeFace( );
  CubeFace(uint8_t, CRGB::HTMLColorCode);

  void rotateCW( );
  void rotateCCW( );

  void setFace(CRGB::HTMLColorCode[3][3]);

  void setUpperFaceBorders(CubeFace *, CubeFace *, CubeFace *, CubeFace *);
  void setBottomFaceBorders(CubeFace *, CubeFace *, CubeFace *, CubeFace *);
  void setSideFaceBorder(CubeFace *);
};