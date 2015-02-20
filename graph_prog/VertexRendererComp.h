#ifndef _RENDERER_COMP_H
#define _RENDERER_COMP_H

#include <lnfw/components/component.h>
#include <texture_comp.h>
#include <shape_comp.h>
#include <lnfw/physics/transform.h>
#include <winapp_material.h>

namespace winapp {

  class VertexRendererComp : public lnfw::Component
  {
  public:
    // Register the component
    REGISTER_COMPONENT(VertexRendererComp);

    const void render(const lnfw::Transform<Vec3> *transform, 
                              const ShapeComp *shape, 
                              const TextureComp *texture = NULL,
                              const MaterialComp *material = NULL);
  };
}
// EO Namespace

#endif