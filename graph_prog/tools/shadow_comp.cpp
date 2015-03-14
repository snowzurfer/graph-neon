
// Includes
#include <shadow_comp.h>

namespace winapp {
  REGISTER_COMPONENT_IMPL(ShadowComp);

  ShadowComp::ShadowComp(const std::vector<const Light *> lights) :
    lights_(lights)
  {

  }

}
// EO Namespace