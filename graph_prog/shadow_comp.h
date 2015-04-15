#ifndef _SHADOW_COMP_H
#define _SHADOW_COMP_H

// Includes
#include <lnfw/components/component.h>
#include <light.h>
#include <vector>

namespace winapp {

  class ShadowComp : public lnfw::Component
  {
  public:
    // Register the class
    REGISTER_COMPONENT(ShadowComp);

    // Ctor
    ShadowComp(){};


  };
  // EO Class

}
// EO Namespace


#endif