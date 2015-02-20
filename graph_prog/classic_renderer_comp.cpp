
// Includes
#include <classic_renderer_comp.h>
#include <Windows.h>
#include <gl/GL.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(ClassicRendererComp);

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
    glScalef(transform->rotation.getX(),
      transform->rotation.getY(),
      transform->rotation.getZ());

    // Set material properties for the geometry
    material->apply(GL_FRONT);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    //////////RENDER

    // Begin drawing
    glBegin(GL_TRIANGLES);

    // For each index
    for(int i = 0; i < shape->getIndices().size(); ++i) {
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

  }

}
// EO Namespace