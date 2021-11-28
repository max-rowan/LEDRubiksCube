#include <Arduino.h>
#include <RubiksCubeSolver.h>

RubiksCube cube = RubiksCube( );
RubiksCubeSolver solver = RubiksCubeSolver(&cube);
CubeDisplay cd;

void setup( ) {
  cd.pCube = &cube;
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(cd.leds, NUM_LEDS);
  FastLED.setBrightness(5);

  // cube.f( );
  // cube.b( );
  // cube.u( );
  // cube.d( );
  // cd.display( );

  // solver.step1( );
  // solver.step2( );
  // solver.step3( );
  // solver.step4( );
  // solver.step5( );
  // solver.step6( );
  // solver.step7( );
  // solver.step8( );
}

void loop( ) {
  delay(1000);
  // cd.rotate(cube.pFront, false, 1000);
  // cube.f( );
  cd.rotate(cube.pRight, false, 1000);
  cube.r( );
  // cd.rotate(cube.pBack, false, 1000);
  // cube.b( );
  // cd.rotate(cube.pLeft, false, 1000);
  // cube.l( );
  // cd.rotate(cube.pUpper, false, 1000);
  // cube.u( );
  // cd.rotate(cube.pBottom, false, 1000);
  // cube.d( );
  cd.display( );
}
