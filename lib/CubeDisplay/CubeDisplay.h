#ifndef CUBE_DISPLAY_H
#define CUBEDISPLAY_H

#define FASTLED_INTERNAL
#include <DisplayUtil.h>
#include <EdgeController.h>
#include <RubiksCube.h>
class CubeDisplay {
private:
  static DisplayUtil _du;
  static EdgeController _ec;

  static void _clearFaceLEDs(CubeFace);
  static void _setFaceLEDs(CubeFace *, double);

public:
  static CRGB leds[NUM_LEDS];
  static RubiksCube *pCube;
  static void init( );
  static void setCubeLEDs(CubeFace *, double);
  static void setCubeLEDs( );
  static void rotate(CubeFace *, boolean, int);
  static void display( );
};

#endif