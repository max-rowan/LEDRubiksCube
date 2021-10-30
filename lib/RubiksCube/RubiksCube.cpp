#include <RubiksCube.h>

RubiksCube::RubiksCube( )
    : front(0, CRGB::Red), right(1, CRGB::Blue), back(2, CRGB::Orange), left(3, CRGB::Green),
      upper(4, CRGB::White), bottom(5, CRGB::Yellow), pFront(&front), pRight(&right), pBack(&back),
      pLeft(&left), pUpper(&upper), pBottom(&bottom) {
  this->_initBorders( );
}

void RubiksCube::_initBorders( ) {
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
void RubiksCube::turnCube( ) {
  CubeFace *pOldFront = pFront;
  pFront = pRight;
  pRight = pBack;
  pBack = pLeft;
  pLeft = pOldFront;
}

void RubiksCube::turnCube_( ) {
  CubeFace *pOldFront = pFront;
  pFront = pLeft;
  pLeft = pBack;
  pBack = pRight;
  pRight = pOldFront;
}

void RubiksCube::f( ) { pFront->rotateCW( ); }

void RubiksCube::f_( ) { pFront->rotateCCW( ); }

void RubiksCube::r( ) { pRight->rotateCW( ); }

void RubiksCube::r_( ) { pRight->rotateCCW( ); }

void RubiksCube::b( ) { pBack->rotateCCW( ); }

void RubiksCube::b_( ) { pBack->rotateCW( ); }

void RubiksCube::l( ) { pLeft->rotateCW( ); }

void RubiksCube::l_( ) { pLeft->rotateCCW( ); }

void RubiksCube::u( ) { pUpper->rotateCW( ); }

void RubiksCube::u_( ) { pUpper->rotateCCW( ); }

void RubiksCube::d( ) { pBottom->rotateCW( ); }

void RubiksCube::d_( ) { pBottom->rotateCCW( ); }
