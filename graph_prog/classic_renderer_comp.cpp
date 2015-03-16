
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
    // Setup rendering; performs type-of-renderer-agnostic steps
    setupRendering(transform, shape, texture, material);

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

    cleanUpTextures(texture);
  }

}
// EO Namespace