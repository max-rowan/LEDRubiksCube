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
  int prevLed = NUM_LEDS - 1;

  for (int side = 0; side < 5; side++) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        int ystart = side * 8;
        int led = cd._XY(x, ystart + y);

        cd.leds[prevLed] = CRGB::Black;
        cd.leds[led] = CRGB::Red;

        prevLed = led;

        FastLED.show( );
        delay(100);
      }
    }
  }
}