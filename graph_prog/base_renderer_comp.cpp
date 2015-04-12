
// Includes
#include <base_renderer_comp.h>
#include <Windows.h>
#include <gl\GL.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(BaseRendererComp);

  void applyGeometryTransform(const lnfw::Transform<Vec3> *transform) {
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

  void applyTextureTransform
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
  
  void cleanUpTextures(const TextureComp *tComp) {
    // If there there is a texture component
    if(tComp) {
      glMatrixMode(GL_TEXTURE);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }
    else {
      // Texturing had been disabled; enable it again
      glEnable(GL_TEXTURE_2D);
    }
  }

  void setupRendering(const lnfw::Transform<Vec3> *transform, 
    const ShapeComp *shape, const TextureComp *texture, const MaterialComp *material) {
      // Apply geometry transform
      applyGeometryTransform(transform);

      // If there is a material
      if(material) {
        // Set material properties for the geometry
        material->apply(GL_FRONT);
      }

      // Activate blending
      glEnable(GL_BLEND);

      // If there is a texture
      if(texture) {
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

        // Apply textire transforms
        applyTextureTransform(texture);
      }
      else {
        // Disable texturing
        glDisable(GL_TEXTURE_2D);
      }

      // If there is a shape
      if(shape) {
        // Setup rendering direction
        glFrontFace(shape->getRenderingDir());
      }
  }

}