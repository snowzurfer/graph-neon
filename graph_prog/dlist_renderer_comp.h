#ifndef _DLIST_RENDERER_COMP_H
#define _DLIST_RENDERER_COMP_H

#include <lnfw/components/component.h>
#include <texture_comp.h>
#include <shape_comp.h>
#include <lnfw/physics/transform.h>
#include <material_comp.h>
#include <base_renderer_comp.h>

namespace winapp {

  class DListRendererComp : public BaseRendererComp
  {
  public:

    const void render(const lnfw::Transform<Vec3> *transform, 
      const ShapeComp *shape, 
      const TextureComp *texture = NULL,
      const MaterialComp *material = NULL);
  };
  // EO Class

}
// EO Namespace

#endif