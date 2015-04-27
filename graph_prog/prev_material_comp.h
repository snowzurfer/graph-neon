#ifndef _WINAPP_PREV_MATERIAL_H
#define _WINAPP_PREV_MATERIAL_H

#include <windows.h>
#include <gl/GL.h>
#include <winapp_colour.h>
#include <cstdint>
#include <lnfw/components/component.h>
#include <material_comp.h>

namespace winapp {

  class PrevMaterialComp : public lnfw::Component
  {
  public:
    // Register the component
    REGISTER_COMPONENT(PrevMaterialComp);

    // Ctor
    PrevMaterialComp(const MaterialComp &Mat) :
      mat(Mat) {};

    // The material to save for later
    MaterialComp mat;

  };


}
// EO Namespace

#endif