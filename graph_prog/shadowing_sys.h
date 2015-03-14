#ifndef _SHADOWING_SYS_H
#define _SHADOWING_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>
#include <light.h>
#include <shape_comp.h>

namespace winapp {

  class ShadowingSys : public lnfw::System
  {
  public:
    // Render shadows for the entities which have a shadow component
    void update(const std::list<lnfw::Entity *> &entities);

  private:
    // Render shadow polys
    void doShadowPass_(const ShapeComp &shapeComp, const Light &light);
  };
  // EO Class

}
// EO Namespace

#endif