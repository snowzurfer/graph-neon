#ifndef _WINAPP_COLOUR_H
#define _WINAPP_COLOUR_H

#include <windows.h>
#include <gl/GL.h>

namespace winapp {

class Colour
{
public:

  // Ctor
  Colour();

  // Ctor with array-defined colour
  Colour(const GLfloat *glColour);

  // Ctor
  Colour(const GLfloat R, const GLfloat G, const GLfloat B,
    const GLfloat A);

  // RGBA values
  float r,g,b,a;

  // Implicit conversion operator overload
  operator GLfloat* () const;
  operator const GLfloat* () const;

};
// EO Class

// Define some colours
const Colour kBlackColourNoAlpha(0.f, 0.f, 0.f, 1.f);
const Colour kWhiteColourNoAlpha(1.f, 1.f, 1.f, 1.f);

}
// EO Namespace

#endif