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
    ShadowComp(const std::vector<const Light *> lights);

    // Getters and setters
    std::vector<const Light *> getLights() const { 
      return lights_;
    }
    void setLights(std::vector<const Light *> val) {
      lights_ = val;
    }

  private:
    // List of references to lights
    std::vector<const Light *> lights_;

  };
  // EO Class

}
// EO Namespace


#endif