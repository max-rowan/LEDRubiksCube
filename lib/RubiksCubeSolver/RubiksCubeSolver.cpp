#include <RubiksCubeSolver.h>

RubiksCubeSolver::RubiksCubeSolver(RubiksCube* pCube) : cube(pCube) {
  cd.init();
}

/************************ Movement Wrappers *********************/

void RubiksCubeSolver::_f() {
  delay(200);
  cd.rotate(cube->pFront, false, 500);
  cube->f();
  cd.display();
}

void RubiksCubeSolver::_f_() {
  delay(200);
  cd.rotate(cube->pFront, true, 500);
  cube->f_();
  cd.display();
}

void RubiksCubeSolver::_l() {
  delay(200);
  cd.rotate(cube->pLeft, false, 500);
  cube->l();
  cd.display();
}

void RubiksCubeSolver::_l_() {
  delay(200);
  cd.rotate(cube->pLeft, true, 500);
  cube->l_();
  cd.display();
}

void RubiksCubeSolver::_b() {
  delay(200);
  cd.rotate(cube->pBack, false, 500);
  cube->b();
  cd.display();
}

void RubiksCubeSolver::_b_() {
  delay(200);
  cd.rotate(cube->pBack, true, 500);
  cube->b_();
  cd.display();
}

void RubiksCubeSolver::_r() {
  delay(200);
  cd.rotate(cube->pRight, false, 500);
  cube->r();
  cd.display();
}

void RubiksCubeSolver::_r_() {
  delay(200);
  cd.rotate(cube->pRight, true, 500);
  cube->r_();
  cd.display();
}

void RubiksCubeSolver::_u() {
  delay(200);
  cd.rotate(cube->pUpper, false, 500);
  cube->u();
  cd.display();
}

void RubiksCubeSolver::_u_() {
  delay(200);
  cd.rotate(cube->pUpper, true, 500);
  cube->u_();
  cd.display();
}

void RubiksCubeSolver::_d() {
  delay(200);
  cd.rotate(cube->pBottom, false, 500);
  cube->d();
  cd.display();
}

void RubiksCubeSolver::_d_() {
  delay(200);
  cd.rotate(cube->pBottom, true, 500);
  cube->d_();
  cd.display();
}

bool RubiksCubeSolver::_isCross(CubeFace cf, CRGB::HTMLColorCode color) {
  return cf.face[0][1] == color && cf.face[1][0] == color &&
         cf.face[1][2] == color && cf.face[2][1] == color;
}

bool RubiksCubeSolver::_isSolid(CubeFace cf) {
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      if (cf.face[y][x] != cf.color) return false;
    }
  }
  return true;
}

/************************ Step 1 - Create (Bottom) Daisy *********************/

void RubiksCubeSolver::_s1BottomEMid() {
  // check bottom border edge, middle panel
  if (*cube->pFront->bottomBorderEdge[1] != cube->pBottom->color) return;

  while (*cube->pFront->upperBorderEdge[1] == cube->pBottom->color) {
    if (*cube->pRight->upperBorderEdge[1] != cube->pBottom->color)
      _u();
    else
      _u_();
  }
  _f();
  _f();
}

void RubiksCubeSolver::_s1BottomMid() {
  // check face's bottom row, middle panel
  if (cube->pFront->face[2][1] == cube->pBottom->color) {
    // rotate top until there is no white square in rotation path
    while (*cube->pFront->upperBorderEdge[1] == cube->pBottom->color) {
      if (*cube->pRight->upperBorderEdge[1] != cube->pBottom->color)
        _u();
      else
        _u_();
    }
    _f();
  }
}

void RubiksCubeSolver::_s1MidRight() {
  // check face's middle row, right panel
  if (cube->pFront->face[1][2] == cube->pBottom->color) {
    // rotate top until there is no bottom color square in rotation path
    while (*cube->pRight->upperBorderEdge[1] == cube->pBottom->color) {
      if (*cube->pBack->upperBorderEdge[1] != cube->pBottom->color)
        _u();
      else
        _u_();
    }
    _r();
  }
}

void RubiksCubeSolver::_s1MidLeft() {
  // check middle row, left panel
  if (cube->pFront->face[1][0] == cube->pBottom->color) {
    // rotate top until there is no white square in rotation path
    while (*cube->pLeft->upperBorderEdge[1] == cube->pBottom->color) {
      if (*cube->pBack->upperBorderEdge[1] != cube->pBottom->color)
        _u_();
      else
        _u();
    }
    _l_();
  }
}

void RubiksCubeSolver::_s1TopMid() {
  // check top row, middle panel
  if (cube->pFront->face[2][1] == cube->pBottom->color) {
    // rotate top until there is no bottom color square in rotation path
    while (*cube->pRight->upperBorderEdge[1] == cube->pBottom->color) {
      if (*cube->pBack->upperBorderEdge[1] != cube->pBottom->color)
        _u();
      else
        _u_();
    }
    _f();
    _u_();
    _r();
  }
}

void RubiksCubeSolver::step1() {
  while (!_isCross(*cube->pUpper, cube->pBottom->color)) {
    _s1BottomEMid();
    _s1BottomMid();
    _s1MidRight();
    _s1MidLeft();
    _s1TopMid();

    cube->turnCube();
  }
}

/************************ Step 2 - Create Bottom Cross *********************/

void RubiksCubeSolver::_s2FindEdge() {
  for (int i = 0; i < 4; i++) {
    if (_s2IsEdge()) return;
    _u();
  }
}

bool RubiksCubeSolver::_s2IsEdge() {
  return cube->pFront->face[0][1] == cube->pFront->color &&
         *cube->pFront->upperBorderEdge[1] == cube->pBottom->color;
}

void RubiksCubeSolver::step2() {
  while (!_isCross(cube->bottom, cube->pBottom->color)) {
    _s2FindEdge();
    if (_s2IsEdge()) {
      _f();
      _f();
    }
    cube->turnCube();
  }
}

/************************ Step 3 - Solve Bottom Corners ********************/

bool RubiksCubeSolver::_validBottomCorners() {
  CRGB::HTMLColorCode color = cube->pBottom->color;

  // Bottom face's corners
  if (cube->pBottom->face[0][0] != color ||
      cube->pBottom->face[0][2] != color ||
      cube->pBottom->face[2][0] != color || cube->pBottom->face[2][2] != color)
    return false;

  // Side faces' bottom corners
  if (cube->pFront->face[2][0] != cube->pFront->color ||
      cube->pFront->face[2][2] != cube->pFront->color ||
      cube->pRight->face[2][0] != cube->pRight->color ||
      cube->pRight->face[2][2] != cube->pRight->color ||
      cube->pBack->face[2][0] != cube->pBack->color ||
      cube->pBack->face[2][2] != cube->pBack->color ||
      cube->pLeft->face[2][0] != cube->pLeft->color ||
      cube->pLeft->face[2][2] != cube->pLeft->color)
    return false;

  return true;
}

bool RubiksCubeSolver::_isIncorrectBottomCorner() {
  return *cube->pFront->bottomBorderEdge[0] == cube->pBottom->color &&
         cube->pFront->face[2][2] != cube->pFront->color;
}

void RubiksCubeSolver::_s3DoWrongBottomCorner() {
  // action 1 - incorrect bottom (right) corner
  if (_isIncorrectBottomCorner()) {
    _r();
    _u();
    _r_();
    cube->turnCube_();
  }
}

bool RubiksCubeSolver::_isTopCorner() {
  return cube->pFront->face[0][2] == cube->pBottom->color ||
         *cube->pFront->upperBorderEdge[2] == cube->pBottom->color ||
         *cube->pFront->rightBorderEdge[0] == cube->pBottom->color;
}

void RubiksCubeSolver::_s3FindTopCorner() {
  // action 2 - find a corner piece with a bottom color tile in the top layer
  for (int i = 0; i < 4; i++) {
    if (_isTopCorner()) return;
    cube->turnCube();
  }
}

void RubiksCubeSolver::_s3FacingTop() {
  // If it's facing the top, change to front
  if (*cube->pFront->upperBorderEdge[2] == cube->pBottom->color) {
    while (cube->pFront->face[0][2] != cube->pRight->color) {
      _u();
      cube->turnCube_();
    }
    // change from facing up to facing front (right)
    _f_();
    _u();
    _f();
    _u_();
    _u_();
  }
}

void RubiksCubeSolver::_s3FacingFront() {
  // rotate the upper face until the corner is between the two matching colored
  // center tiles
  if (cube->pFront->face[0][2] == cube->pBottom->color) {
    while (cube->pRight->face[0][0] != cube->pRight->color) {
      _u();
      cube->turnCube_();
    }
    _u();
    _r();
    _u_();
    _r_();
  }
}

void RubiksCubeSolver::_s3FacingRight() {
  // If it's on the left (of the face to the right)
  if (*cube->pFront->rightBorderEdge[0] == cube->pBottom->color) {
    while (cube->pFront->face[0][2] != cube->pFront->color) {
      _u();
      cube->turnCube_();
    }
    cube->turnCube();
    _u_();
    _l_();
    _u();
    _l();
  }
}

void RubiksCubeSolver::step3() {
  while (!_validBottomCorners()) {
    _s3DoWrongBottomCorner();

    _s3FindTopCorner();

    _s3FacingTop();
    _s3FacingFront();
    _s3FacingRight();

    cube->turnCube();
  }
}

/************************ Step 4 - Solve Middle Layer *********************/

bool RubiksCubeSolver::_s4IsFaceMiddleLayer() {
  return cube->pFront->face[1][0] == cube->pFront->color &&
         cube->pFront->face[1][2] == cube->pFront->color &&
         !_s4IsWrongMidRight();
}

bool RubiksCubeSolver::_s4IsCubeMiddleLayer() {
  for (int i = 0; i < 4; i++) {
    if (!_s4IsFaceMiddleLayer()) {
      for (int j = i; j > 0; j--) cube->turnCube_();
      return false;
    };
    cube->turnCube();
  }
  return true;
}

bool RubiksCubeSolver::_s4IsRotateRight() {
  return cube->pFront->face[0][1] == cube->pFront->color &&
         *cube->pFront->upperBorderEdge[1] == cube->pRight->color;
}

bool RubiksCubeSolver::_s4IsRotateLeft() {
  return cube->pFront->face[0][1] == cube->pFront->color &&
         *cube->pFront->upperBorderEdge[1] == cube->pLeft->color;
}

bool RubiksCubeSolver::_s4IsWrongMidRight() {
  return cube->pFront->face[1][2] != cube->pFront->color ||
         cube->pRight->face[1][0] != cube->pRight->color;
}

bool RubiksCubeSolver::_s4IsWrongMidLeft() {
  return cube->pFront->face[1][0] != cube->pFront->color ||
         cube->pLeft->face[1][2] != cube->pLeft->color;
}

void RubiksCubeSolver::_s4RotateMidRight() {
  _u();
  _r();
  _u_();
  _r_();

  _u_();
  _f_();
  _u();
  _f();
}

void RubiksCubeSolver::_s4RotateMidLeft() {
  _u_();
  _l_();
  _u();
  _l();

  _u();
  _f();
  _u_();
  _f_();
}

void RubiksCubeSolver::step4() {
  while (!_s4IsCubeMiddleLayer()) {
    // upper row panel
    // rotate lower face until front matches face & top matches a side color
    for (int i = 0; i < 4; i++) {
      if (_s4IsRotateRight()) {
        _s4RotateMidRight();
        i = 0;
      }
      if (_s4IsRotateLeft()) {
        _s4RotateMidLeft();
        i = 0;
      }
      _u();
    }

    // wrong middle panel
    if (_s4IsWrongMidRight()) {
      _s4RotateMidRight();
    }
    if (_s4IsWrongMidLeft()) {
      _s4RotateMidLeft();
    }

    cube->turnCube();
  }
}

/************************ Step 5 - Solve Upper Cross *********************/
// step 5
bool RubiksCubeSolver::_s5IsLine() {
  return *cube->pLeft->upperBorderEdge[1] == cube->pUpper->color &&
         *cube->pRight->upperBorderEdge[1] == cube->pUpper->color;
}

bool RubiksCubeSolver::_s5IsL() {
  return *cube->pBack->upperBorderEdge[1] == cube->pUpper->color &&
         *cube->pLeft->upperBorderEdge[1] == cube->pUpper->color;
}

void RubiksCubeSolver::step5() {
  while (!_isCross(*cube->pUpper, cube->pUpper->color)) {
    for (int i = 0; i < 4; i++) {
      if (_s5IsLine() || _s5IsL()) continue;
      cube->turnCube();
    }
    _f();
    _u();
    _r();
    _u_();
    _r_();
    _f_();
  }
}

/************************ Step 6 - Orient Corners *********************/

int RubiksCubeSolver::_s6NumFaceCorners(CubeFace cf) {
  return (cf.face[0][0] == cf.color) + (cf.face[0][2] == cf.color) +
         (cf.face[2][0] == cf.color) + (cf.face[2][2] == cf.color);
}

void RubiksCubeSolver::_s6Alg() {
  _r();
  _u();
  _r_();
  _u();
  _r();
  _u();
  _u();
  _r_();
}

void RubiksCubeSolver::step6() {
  while (!_isSolid(cube->upper)) {
    if (_s6NumFaceCorners(cube->upper) == 0) {
      while (cube->pLeft->face[0][2] != cube->pUpper->color) cube->turnCube();
      _s6Alg();
    }

    if (_s6NumFaceCorners(cube->upper) == 1) {
      while (*cube->pFront->upperBorderEdge[0] != cube->pUpper->color)
        cube->turnCube();
      _s6Alg();
    }

    if (_s6NumFaceCorners(cube->upper) == 2) {
      while (cube->pFront->face[0][0] != cube->pUpper->color) cube->turnCube();
      _s6Alg();
    }
  }
}

/************************ Step 7 - Place Upper Corners *********************/

bool RubiksCubeSolver::_s7IsMatchingCorners(CubeFace* cf) {
  return cf->face[0][0] == cf->face[0][2];
}

bool RubiksCubeSolver::_s7IsCornersSolved() {
  for (int i = 0; i < 4; i++) {
    if (!_s7IsMatchingCorners(cube->pFront)) return false;
    cube->turnCube();
  }
  return true;
}

void RubiksCubeSolver::_s7FindDoubledFace() {
  while (!_s7IsMatchingCorners(cube->pFront)) {
    cube->turnCube();
  }
}

void RubiksCubeSolver::_s7MatchDoubledPanels() {
  while (cube->pFront->face[0][0] != cube->pFront->color) {
    _u();
    cube->turnCube_();
  }
  cube->turnCube_();
  cube->turnCube_();
}

void RubiksCubeSolver::_s7Alg() {
  _r_();
  _f();
  _r_();
  _b();
  _b();
  _r();
  _f_();
  _r_();
  _b();
  _b();
  _r();
  _r();
  _u_();
}

void RubiksCubeSolver::step7() {
  while (!_s7IsCornersSolved()) {
    _s7FindDoubledFace();
    _s7MatchDoubledPanels();
    _s7Alg();
  }
}

/************************ Step 8 - Position Upper Edges *********************/

bool RubiksCubeSolver::_s8IsSolved() {
  return cube->pFront->face[0][1] == cube->pFront->color &&
         cube->pRight->face[0][1] == cube->pRight->color &&
         cube->pBack->face[0][1] == cube->pBack->color &&
         cube->pLeft->face[0][1] == cube->pLeft->color;
}

bool RubiksCubeSolver::_s8IsBackEdge() {
  return cube->pBack->face[0][1] == cube->pBack->color;
}

void RubiksCubeSolver::_s8OrientBackEdge() {
  for (int i = 0; i < 4; i++) {
    if (_s8IsBackEdge()) break;
    cube->turnCube();
  }
}

bool RubiksCubeSolver::_s8IsRight() {
  return cube->pFront->face[0][1] == cube->pRight->color;
}

void RubiksCubeSolver::_s8RightAlg() {
  _f();
  _f();
  _u_();
  _l();
  _r_();
  _f();
  _f();
  _l_();
  _r();
  _u_();
  _f();
  _f();
}

void RubiksCubeSolver::_s8LeftAlg() {
  _f();
  _f();
  _u();
  _l();
  _r_();
  _f();
  _f();
  _l_();
  _r();
  _u();
  _f();
  _f();
}

void RubiksCubeSolver::step8() {
  while (!_s8IsSolved()) {
    _s8OrientBackEdge();
    if (_s8IsRight())
      _s8RightAlg();
    else
      _s8LeftAlg();
  }
}
