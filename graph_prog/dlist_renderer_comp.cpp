
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

    
    // Setup rendering; performs type-of-renderer-agnostic steps
    setupRendering(transform, shape, texture, material);

    // If there is a shape
    if(shape) {
      // Activate blending
      //glEnable(GL_BLEND);

      //////////RENDER
      
      // If the shape doesn't have a display list
      assert(shape->getDList() != -1);

      glCallList(shape->getDList());

      // Deactivate blending
      //glDisable(GL_BLEND);

    }

    // Cleanup
    cleanUpTextures(texture);
  }
}
// EO Namespace