#ifndef _CLASSIC_RENDERER_COMP_H
#define _CLASSIC_RENDERER_COMP_H

#include <lnfw/components/component.h>
#include <texture_comp.h>
#include <shape_comp.h>
#include <lnfw/physics/transform.h>
#include <material_comp.h>

namespace winapp {

  class ClassicRendererComp : public lnfw::Component
  {
  public:
    // Register the component
    REGISTER_COMPONENT(ClassicRendererComp);

    const void render(const lnfw::Transform<Vec3> *transform, 
      const ShapeComp *shape, 
      const TextureComp *texture = NULL,
      const MaterialComp *material = NULL);
  };
  // EO Class

}
// EO Namespace

#endif