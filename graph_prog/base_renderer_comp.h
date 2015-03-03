#ifndef _BASE_RENDERER_COMP_H
#define _BASE_RENDERER_COMP_H


#include <lnfw/components/component.h>
#include <texture_comp.h>
#include <shape_comp.h>
#include <lnfw/physics/transform.h>
#include <material_comp.h>

namespace winapp {

  class BaseRendererComp : public lnfw::Component
  {
  public:
    // Register the component
    REGISTER_COMPONENT(BaseRendererComp);

    virtual const void render(const lnfw::Transform<Vec3> *transform, 
      const ShapeComp *shape,
      const TextureComp *texture = NULL,
      const MaterialComp *material = NULL) = 0;

  protected:
    // Apply transform to shape
    void applyGeometryTransform(const lnfw::Transform<Vec3> *transform,
      const ShapeComp *shape);

    // Apply texture transform to texture
    void applyTextureTransform(const TextureComp *texture);

    // Clean up matrix stack for textures
    void cleanUpTextures();
  };
  // EO Class

}
// EO Namespace

#endif