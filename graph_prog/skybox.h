#ifndef _SKYBOX_H
#define _SKYBOX_H


// Includes
#include "cube_shape.h"
#include "app_globals.h"

namespace winapp {

class Skybox
{
public:
  // Ctor
  Skybox(GLuint skyboxTexture);

  // Copy Ctor (deep copy)
  Skybox(const Skybox &ref);

  // Dtor
  ~Skybox();

  // Render the shape
  void draw();

protected:
  // Cube shape to render geometry + texture
  CubeShape *cubeShape_;

  // Skybox texture
  GLuint skyboxTexture_;

};
// EO Class

}
// EO Namespace


#endif