#ifndef _BASE_SHAPE_H
#define _BASE_SHAPE_H


#include <Windows.h>
#include <MMSystem.h>
#include <math.h>
#include <gl/GL.h>  
#include <gl/GLU.h>

namespace winapp {

class BaseShape
{
public:

  virtual void drawShape() = 0;
protected:
  

};
// EO Class

}
// EO Namespace


#endif