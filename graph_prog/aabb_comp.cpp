
// Includes
#include <aabb_comp.h>

namespace lnfw {

  REGISTER_COMPONENT_IMPL(AABBComp);

  AABBComp::AABBComp(const Vec3 &minLeft, const Vec3 &maxRight) :
    minLeft_(minLeft), maxRight_(maxRight)
  {

  }

}
// EO Namespace