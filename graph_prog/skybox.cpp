
// Includes
#include "skybox.h"


namespace winapp {

Skybox::Skybox(GLuint skyboxTexture) :
 cubeShape_(NULL),
 skyboxTexture_(skyboxTexture)
 {
 cubeShape_ = new CubeShape();
}

Skybox::Skybox(const Skybox &ref) :
 cubeShape_(NULL),
 skyboxTexture_(ref.skyboxTexture_)
 {
  cubeShape_ = new CubeShape();
  *cubeShape_ = *(ref.cubeShape_);
}

Skybox::~Skybox() {
  // Free texture memory
  glDeleteTextures(1, &skyboxTexture_);

  // Delete cube
  delete cubeShape_;
}

void Skybox::draw() {
  // Set culling for the front faces
  glCullFace(GL_FRONT);
  
  // Bind skybox texture
  glBindTexture(GL_TEXTURE_2D, skyboxTexture_);

  // Disable depth test
  glDisable(GL_DEPTH_TEST);
  // Disable lighting
  glDisable(GL_LIGHTING);

  // Render textured cube
  cubeShape_->drawShape();

  // Enable depth test again
  glEnable(GL_DEPTH_TEST);
  // Enable lighting again
  glEnable(GL_LIGHTING);

  // Reset culling for the back faces
  glCullFace(GL_BACK);
}

}
// EO Namespace