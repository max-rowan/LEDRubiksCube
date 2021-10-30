#include <CubeFace.h>

CubeFace::CubeFace( ) { };

CubeFace::CubeFace(uint8_t faceNum, CRGB::HTMLColorCode color) : faceNum(faceNum), color(color) {
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      face[y][x] = CubePanel(color, faceNum, x, y);
    }
  }
}

void CubeFace::rotateCW( ) {
  _rotateFaceCW( );
  _rotateEdgesCW( );
}

void CubeFace::rotateCCW( ) {
  _rotateFaceCCW( );
  _rotateEdgesCCW( );
}

void CubeFace::_rotateFaceCW( ) {
  CRGB::HTMLColorCode tempFace[3][3];
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      tempFace[x][2 - y] = face[y][x].color;
    }
  }
  setFace(tempFace);
}

void CubeFace::_rotateFaceCCW( ) {
  CRGB::HTMLColorCode tempFace[3][3];
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      tempFace[2 - x][y] = face[y][x].color;
    }
  }
  setFace(tempFace);
}

void CubeFace::_rotateEdgesCW( ) {
  CubePanel panelZeroColor = CubePanel(upperBorderEdge[0]->color);
  CubePanel panelOneColor = CubePanel(upperBorderEdge[1]->color);
  CubePanel panelTwoColor = CubePanel(upperBorderEdge[2]->color);

  CubePanel *origUpperEdgeVals[3] = {&panelZeroColor, &panelOneColor, &panelTwoColor};
  _rotateEdge(leftBorderEdge, upperBorderEdge);
  _rotateEdge(bottomBorderEdge, leftBorderEdge);
  _rotateEdge(rightBorderEdge, bottomBorderEdge);
  _rotateEdge(origUpperEdgeVals, rightBorderEdge);
}

void CubeFace::_rotateEdgesCCW( ) {
  CubePanel panelZeroColor = CubePanel(upperBorderEdge[0]->color);
  CubePanel panelOneColor = CubePanel(upperBorderEdge[1]->color);
  CubePanel panelTwoColor = CubePanel(upperBorderEdge[2]->color);

  CubePanel *origUpperEdgeVals[3] = {&panelZeroColor, &panelOneColor, &panelTwoColor};
  _rotateEdge(rightBorderEdge, upperBorderEdge);
  _rotateEdge(bottomBorderEdge, rightBorderEdge);
  _rotateEdge(leftBorderEdge, bottomBorderEdge);
  _rotateEdge(origUpperEdgeVals, leftBorderEdge);
}

void CubeFace::_rotateEdge(CubePanel **fromEdge, CubePanel **toEdge) {
  toEdge[0]->color = fromEdge[0]->color;
  toEdge[1]->color = fromEdge[1]->color;
  toEdge[2]->color = fromEdge[2]->color;
}

void CubeFace::_setBorderEdge(CubePanel **edge, CubePanel *panel0, CubePanel *panel1,
                              CubePanel *panel2) {
  edge[0] = panel0;
  edge[1] = panel1;
  edge[2] = panel2;
}

void CubeFace::setFace(CRGB::HTMLColorCode newFace[3][3]) {
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      face[y][x].color = newFace[y][x];
    }
  }
}

/**
 * Sets the borders of the top face
 *  and the top borders of the other faces
 */
void CubeFace::setUpperFaceBorders(CubeFace *u, CubeFace *r, CubeFace *b, CubeFace *l) {
  // top/upper edge
  _setBorderEdge(upperBorderEdge, &(u->face[0][2]), &(u->face[0][1]), &(u->face[0][0]));
  _setBorderEdge(u->upperBorderEdge, &(face[0][2]), &(face[0][1]), &(face[0][0]));

  // right edge
  _setBorderEdge(rightBorderEdge, &(r->face[0][2]), &(r->face[0][1]), &(r->face[0][0]));
  _setBorderEdge(r->upperBorderEdge, &(face[2][2]), &(face[1][2]), &(face[0][2]));

  // bottom/lower edge
  _setBorderEdge(bottomBorderEdge, &(b->face[0][2]), &(b->face[0][1]), &(b->face[0][0]));
  _setBorderEdge(b->upperBorderEdge, &(face[2][0]), &(face[2][1]), &(face[2][2]));

  // left edge
  _setBorderEdge(leftBorderEdge, &(l->face[0][2]), &(l->face[0][1]), &(l->face[0][0]));
  _setBorderEdge(l->upperBorderEdge, &(face[0][0]), &(face[1][0]), &(face[2][0]));
}

/**
 * Sets the borders of the bottom face
 *  and the bottom borders of the other faces
 */
void CubeFace::setBottomFaceBorders(CubeFace *u, CubeFace *r, CubeFace *b, CubeFace *l) {
  // top/upper edge
  _setBorderEdge(upperBorderEdge, &(u->face[2][0]), &(u->face[2][1]), &(u->face[2][2]));
  _setBorderEdge(u->bottomBorderEdge, &(face[0][2]), &(face[0][1]), &(face[0][0]));

  // right edge
  _setBorderEdge(rightBorderEdge, &(r->face[2][0]), &(r->face[2][1]), &(r->face[2][2]));
  _setBorderEdge(r->bottomBorderEdge, &(face[2][2]), &(face[1][2]), &(face[0][2]));

  // bottom/lower edge
  _setBorderEdge(bottomBorderEdge, &(b->face[2][0]), &(b->face[2][1]), &(b->face[2][2]));
  _setBorderEdge(b->bottomBorderEdge, &(face[2][0]), &(face[2][1]), &(face[2][2]));

  // left edge
  _setBorderEdge(leftBorderEdge, &(l->face[2][0]), &(l->face[2][1]), &(l->face[2][2]));
  _setBorderEdge(l->bottomBorderEdge, &(face[0][0]), &(face[1][0]), &(face[2][0]));
}

/**
 * Sets the right/left borders of a side face
 */
void CubeFace::setSideFaceBorder(CubeFace *r) {
  _setBorderEdge(rightBorderEdge, &(r->face[0][0]), &(r->face[1][0]), &(r->face[2][0]));
  _setBorderEdge(r->leftBorderEdge, &(face[2][2]), &(face[1][2]), &(face[0][2]));
}
