#include <RubiksCube.h>

RubiksCube::RubiksCube()
    : front(CRGB::Red),
      pFront(&front),
      right(CRGB::Blue),
      pRight(&right),
      back(CRGB::Orange),
      pBack(&back),
      left(CRGB::Green),
      pLeft(&left),
      upper(CRGB::White),
      pUpper(&upper),
      bottom(CRGB::Yellow),
      pBottom(&bottom) {
  this->_initBorders();
}

void RubiksCube::_initBorders() {
  upper.setUpperFaceBorders(&back, &right, &front, &left);
  bottom.setBottomFaceBorders(&front, &right, &back, &left);
  front.setSideFaceBorder(&right);
  right.setSideFaceBorder(&back);
  back.setSideFaceBorder(&left);
  left.setSideFaceBorder(&front);
}

/**
 * Turns cube such that the right face becomes the new front
 */
void RubiksCube::turnCube() {
  CubeFace* pOldFront = pFront;
  pFront = pRight;
  pRight = pBack;
  pBack = pLeft;
  pLeft = pOldFront;
}

void RubiksCube::turnCube_() {
  CubeFace* pOldFront = pFront;
  pFront = pLeft;
  pLeft = pBack;
  pBack = pRight;
  pRight = pOldFront;
}

void RubiksCube::f() { pFront->rotateCW(); }

void RubiksCube::f_() { pFront->rotateCCW(); }

void RubiksCube::r() { pRight->rotateCW(); }

void RubiksCube::r_() { pRight->rotateCCW(); }

void RubiksCube::b() { pBack->rotateCCW(); }

void RubiksCube::b_() { pBack->rotateCW(); }

void RubiksCube::l() { pLeft->rotateCW(); }

void RubiksCube::l_() { pLeft->rotateCCW(); }

void RubiksCube::u() { pUpper->rotateCW(); }

void RubiksCube::u_() { pUpper->rotateCCW(); }

void RubiksCube::d() { pBottom->rotateCW(); }

void RubiksCube::d_() { pBottom->rotateCCW(); }
