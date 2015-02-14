
// Includes
#include "winapp_colour.h"


namespace winapp {

Colour::Colour() :
  r(0.f),
  g(0.f),
  b(0.f),
  a(0.f) 
{

}

Colour::Colour(const GLfloat *glColour) :
  r(glColour[0]),
  g(glColour[1]),
  b(glColour[2]),
  a(glColour[3]) 
{

}

Colour::Colour(const GLfloat R, const GLfloat G, const GLfloat B, const GLfloat A) :
  r(R),
  g(G),
  b(B),
  a(A)
{

}

Colour::operator GLfloat *(){
  return (GLfloat*)(this);
};

}
// EO Namespace