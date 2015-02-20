
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

    // Begin Drawing
    glBegin(GL_TRIANGLES);

    // For each index
    for(int i = 0; i < shape->getIndices().size(); ++i) {
      // Get the normal, vertex and texel at index i
      const Vec3 norm = shape->getNormals()[shape->getIndices()[i]];
      const Vec3 vertx = shape->getVertices()[shape->getIndices()[i]];


      glNormal3f(0.f, 0.f, 1.f);
    }

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Specify data for the arrays
    glVertexPointer(3, GL_FLOAT, 0, &shape->getVertices()[0]);
    glNormalPointer(GL_FLOAT, 0, &shape->getNormals()[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &shape->getNormals()[0]);

    // Deference
    glDrawElements(GL_TRIANGLES, shape->getVertices().size(), 
      GL_UNSIGNED_BYTE, &shape->getIndices()[0]);

    // Disable client states
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

}
// EO Namespace