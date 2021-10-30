#include <CubeFace.h>
#include <math.h>

class RubiksCube {
 private:
  CubeFace front;
  CubeFace right;
  CubeFace back;
  CubeFace left;
  CubeFace upper;
  CubeFace bottom;
  void _initBorders();
  void _setPFace();
  static void _assignBorders(CubeFace*, CubeFace*, CubeFace*, CubeFace*,
                             CubeFace*);

 public:
  CubeFace* pFront;
  CubeFace* pRight;
  CubeFace* pBack;
  CubeFace* pLeft;
  CubeFace* pUpper;
  CubeFace* pBottom;

  RubiksCube();
  void turnCube();
  void turnCube_();
  void f();
  void f_();
  void l();
  void l_();
  void b();
  void b_();
  void r();
  void r_();
  void u();
  void u_();
  void d();
  void d_();
};
