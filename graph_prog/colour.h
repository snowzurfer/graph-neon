#ifndef _APPCOLOUR_H
#define _APPCOLOUR_H


// Includes
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

namespace winapp {

class Colour
{
public:
  // Default ctor
  Colour();

  // Ctor with array-defined colour
  Colour(const GLfloat *glColour);

  // Ctor
  Colour(const GLfloat R, const GLfloat G, const GLfloat B, 
    const GLfloat A);

  // RGBA values
  float r,g,b,a;

  // Implicit conversion operator overload
  operator GLfloat* ();
};
// EO Class


// Define some colours
const Colour kBlackColourNoAlpha(0.f, 0.f, 0.f, 1.f);
const Colour kWhiteColourNoAlpha(1.f, 1.f, 1.f, 1.f);

}
// EO Namespace

#endif