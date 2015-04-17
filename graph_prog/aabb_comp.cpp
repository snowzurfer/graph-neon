
// Includes
#include <aabb_comp.h>

namespace lnfw {

  REGISTER_COMPONENT_IMPL(AABBComp);

  AABBComp::AABBComp(const Vec3 &minLeft, const Vec3 &maxRight) :
    Component(),
    minLeft_(minLeft), maxRight_(maxRight)
  {

  }

  AABBComp::AABBComp(const std::vector<Vec3> &vertices) :
    Component(), minLeft_(1000.f, 1000.f, 1000.f), 
    maxRight_(-1000.f, -1000.f, -1000.f)
  {
    // For each vertex
    for(int i = 0; i < vertices.size(); ++i) {
      // If it is larger than the current largest
      if(maxRight_ < vertices[i]) {
        maxRight_ = vertices[i];
      }

      // If it is larger than the current smallest
      if(minLeft_ > vertices[i]) {
        minLeft_ = vertices[i];
      }
    }
  }

}
// EO Namespace