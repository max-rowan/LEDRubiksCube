#include <CubeFace.h>

CubeFace::CubeFace() { };

CubeFace::CubeFace(CRGB::HTMLColorCode color) {
  this->color = color;
  for (uint8_t y=0;y<3;y++) {
    for (uint8_t x=0;x<3;x++) {
      this->face[y][x] = color;
    }
  }
}

void CubeFace::rotateCW() {
  this->_rotateFaceCW();
  this->_rotateEdgesCW();
}

void CubeFace::rotateCCW() {
  this->_rotateFaceCCW();
  this->_rotateEdgesCCW();
}

void CubeFace::_rotateFaceCW() {
  CRGB::HTMLColorCode tempFace[3][3];
  for (uint8_t y=0;y<3;y++) {
    for (uint8_t x=0;x<3;x++) {
      tempFace[x][2-y] = face[y][x];
    }
  }  
  this->setFace(tempFace);
}

void CubeFace::_rotateFaceCCW() {
  CRGB::HTMLColorCode tempFace[3][3];
  for (uint8_t y=0;y<3;y++) {
    for (uint8_t x=0;x<3;x++) {
      tempFace[2-x][y] = face[y][x];
    }
  }  
  this->setFace(tempFace);
}

void CubeFace::_rotateEdgesCW() {
  CRGB::HTMLColorCode panelZeroColor = *upperBorderEdge[0];
  CRGB::HTMLColorCode panelOneColor = *upperBorderEdge[1];
  CRGB::HTMLColorCode panelTwoColor = *upperBorderEdge[2];

  CRGB::HTMLColorCode* origUpperEdgeVals[3] = {&panelZeroColor, &panelOneColor, &panelTwoColor};
  _rotateEdge(leftBorderEdge, upperBorderEdge);
  _rotateEdge(bottomBorderEdge, leftBorderEdge);
  _rotateEdge(rightBorderEdge, bottomBorderEdge);
  _rotateEdge(origUpperEdgeVals, rightBorderEdge);
}

void CubeFace::_rotateEdgesCCW() {
  CRGB::HTMLColorCode panelZeroColor = *upperBorderEdge[0];
  CRGB::HTMLColorCode panelOneColor = *upperBorderEdge[1];
  CRGB::HTMLColorCode panelTwoColor = *upperBorderEdge[2];

  CRGB::HTMLColorCode* origUpperEdgeVals[3] = {&panelZeroColor, &panelOneColor, &panelTwoColor};
  _rotateEdge(rightBorderEdge, upperBorderEdge);
  _rotateEdge(bottomBorderEdge, rightBorderEdge);
  _rotateEdge(leftBorderEdge, bottomBorderEdge);
  _rotateEdge(origUpperEdgeVals, leftBorderEdge);
}

void CubeFace::_rotateEdge(CRGB::HTMLColorCode** fromEdge, CRGB::HTMLColorCode** toEdge) {
  *toEdge[0] = *fromEdge[0];
  *toEdge[1] = *fromEdge[1];
  *toEdge[2] = *fromEdge[2];
}

void CubeFace::_setBorderEdge(CRGB::HTMLColorCode** edge, CRGB::HTMLColorCode* panel0, CRGB::HTMLColorCode* panel1, CRGB::HTMLColorCode* panel2) {
  edge[0] = panel0;
  edge[1] = panel1;
  edge[2] = panel2;
}

void CubeFace::setFace(CRGB::HTMLColorCode newFace[3][3]) {
  for (uint8_t y=0;y<3;y++) {
    for (uint8_t x=0;x<3;x++) {
      face[y][x] = newFace[y][x];
    }
  }
}

/**
 * Sets the borders of the top face
 *  and the top borders of the other faces
 */
void CubeFace::setUpperFaceBorders(CubeFace* u, CubeFace* r, CubeFace* b, CubeFace* l) {
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
void CubeFace::setBottomFaceBorders(CubeFace* u, CubeFace* r, CubeFace* b, CubeFace* l) {
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
void CubeFace::setSideFaceBorder(CubeFace* r) {
  _setBorderEdge(rightBorderEdge, &(r->face[0][0]), &(r->face[1][0]), &(r->face[2][0]));
  _setBorderEdge(r->leftBorderEdge, &(face[2][2]), &(face[1][2]), &(face[0][2]));
}
