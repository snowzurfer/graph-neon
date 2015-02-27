
// Includes
#include <classic_renderer_comp.h>
#include <Windows.h>
#include <gl/GL.h>

namespace winapp {

  const void ClassicRendererComp::render(const lnfw::Transform<Vec3> *transform, 
    const ShapeComp *shape, 
    const TextureComp *texture /* = NULL */,
    const MaterialComp *material /* = NULL */)
  {
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

    // Set material properties for the geometry
    material->apply(GL_FRONT);

    // Activate blending
    glEnable(GL_BLEND);

    //// Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    //////////RENDER

    // Begin drawing
    glBegin(GL_TRIANGLES);

    // For each index
    for(unsigned int i = 0; i < shape->getIndices().size(); ++i) {
      // Get the normal, vertex and texel at index i
      const Vec3 norm = shape->getNormals()[shape->getIndices()[i]];
      const Vec3 vertx = shape->getVertices()[shape->getIndices()[i]];
      const Texel texel = shape->getTexels()[shape->getIndices()[i]];

      // Apply these attributes and render
      glNormal3f(norm.getX(), norm.getY(), norm.getY());
      glTexCoord2f(texel.x, texel.y);
      glVertex3f(vertx.getX(), vertx.getY(), vertx.getZ());
    }

    // End drawing
    glEnd();

    // Deactivate blending
    glDisable(GL_BLEND);

  }

}
// EO Namespace