#ifndef _DISPLAY_LIST_GEN_H
#define _DISPLAY_LIST_GEN_H

#include <Windows.h>
#include <MMSystem.h>
#include <math.h>
#include <gl/GL.h>  
#include <gl/GLU.h>

namespace winapp {

class DisplayListGen
{
public:
  // Ctor
  DisplayListGen();

  // Dtor
  ~DisplayListGen();
  
  // Generate diplay lists
  GLuint generateDispList();



private:
  // Create unit cube
  GLuint genUnitCube();

  // TODO Create cylinder

  // TODO Create sphere

  // Create textured unit cube
  GLuint genTexUnitCube(GLuint textureID);

  // TODO Create textured cyclinder

  // TODO Create textured sphere

};
// EO Class

}
// EO Namespace

#endif