#include <CubeDisplay.h>
// #include <RubiksCube.h>

const int TURN_SPEED = 100;

class RubiksCubeSolver {
private:
  RubiksCube *cube;
  CubeDisplay cd;

  // movement wrappers
  void _f( );
  void _f_( );
  void _l( );
  void _l_( );
  void _b( );
  void _b_( );
  void _r( );
  void _r_( );
  void _u( );
  void _u_( );
  void _d( );
  void _d_( );

  // multiple
  bool _isCross(CubeFace, CRGB::HTMLColorCode);
  bool _isSolid(CubeFace);

  // step 1
  void _s1BottomEMid( );
  void _s1BottomMid( );
  void _s1MidRight( );
  void _s1MidLeft( );
  void _s1TopMid( );

  // step 2
  void _s2FindEdge( );
  bool _s2IsEdge( );

  // step 3
  bool _validBottomCorners( );
  bool _isIncorrectBottomCorner( );
  void _s3DoWrongBottomCorner( );
  bool _isTopCorner( );
  void _s3FindTopCorner( );
  void _s3FacingTop( );
  void _s3FacingFront( );
  void _s3FacingRight( );

  // step 4
  bool _s4IsRotateRight( );
  bool _s4IsRotateLeft( );
  bool _s4IsFaceMiddleLayer( );
  bool _s4IsCubeMiddleLayer( );
  bool _s4IsWrongMidRight( );
  bool _s4IsWrongMidLeft( );
  void _s4RotateMidRight( );
  void _s4RotateMidLeft( );

  // step 5
  bool _s5IsLine( );
  bool _s5IsL( );

  // step 6
  int _s6NumFaceCorners(CubeFace);
  void _s6Alg( );

  // step 7
  bool _s7IsMatchingCorners(CubeFace *);
  bool _s7IsCornersSolved( );
  void _s7FindDoubledFace( );
  void _s7MatchDoubledPanels( );
  void _s7Alg( );

  // step 8
  bool _s8IsSolved( );
  bool _s8IsBackEdge( );
  void _s8OrientBackEdge( );
  bool _s8IsRight( );
  void _s8RightAlg( );
  void _s8LeftAlg( );

public:
  RubiksCubeSolver(RubiksCube *);
  void step1( );
  void step2( );
  void step3( );
  void step4( );
  void step5( );
  void step6( );
  void step7( );
  void step8( );
};
