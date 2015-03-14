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
    ShadowComp(std::vector<Light *> &lights);

    // Getters and setters
    const std::vector<Light *> &getLights() const { 
      return lights_;
    }
    void setLights(std::vector<Light *> val) {
      lights_ = val;
    }

  private:
    // List of references to lights
    std::vector<Light *> &lights_;

  };
  // EO Class

}
// EO Namespace


#endif