
// Includes
#include <base_renderer_comp.h>
#include <Windows.h>
#include <gl\GL.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(BaseRendererComp);

  void BaseRendererComp::applyGeometryTransform(const lnfw::Transform<Vec3> *transform,
      const ShapeComp *shape) {
    // Move to position
    glTranslatef(transform->position.getX(),
                 transform->position.getY(),
                 transform->position.getZ());
    // Scale
    glScalef(transform->scale.getX(),
             transform->scale.getY(),
             transform->scale.getZ());

    // Rotate
    glRotatef(transform->rotation.getX(), 1.f, 0.f, 0.f);
    glRotatef(transform->rotation.getY(), 0.f, 1.f, 0.f);
    glRotatef(transform->rotation.getZ(), 0.f, 0.f, 1.f);
  }

  void BaseRendererComp::applyTextureTransform
    (const TextureComp *texture) {
    // If there is a texture
    if(texture) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Default texture behaviour
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      // Select the correct matrix
      glMatrixMode(GL_TEXTURE);
      // Push the matrix
      glPushMatrix();

        // Move to position
        glTranslatef(texture->getTransform().position.x,
                   texture->getTransform().position.y,
                   0.f);
        // Scale
        glScalef(texture->getTransform().scale.x,
                 texture->getTransform().scale.y,
                 1.f);

        // Rotate
        glRotatef(texture->getTransform().rotation.x, 1.f, 0.f, 0.f);
        glRotatef(texture->getTransform().rotation.y, 0.f, 1.f, 0.f);
        //glRotatef(0.f, 0.f, 0.f, 1.f);

      //glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }
  }

  void BaseRendererComp::cleanUpTextures() {
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }

}