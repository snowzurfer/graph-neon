
// Includes
#include <dlist_renderer_comp.h>
#include <Windows.h>
#include <gl/GL.h>
#include <cassert>

namespace winapp {

  const void DListRendererComp::render(const lnfw::Transform<Vec3> *transform, 
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
    
    // If the shape doesn't have a display list
    assert(shape->getDList() == -1);

    glCallList(shape->getDList());


  }

}
// EO Namespace