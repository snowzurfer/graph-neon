
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
      // If it is larger than the current largest, for each component
      if(maxRight_.getX() < vertices[i].getX()) {
        maxRight_.setX(vertices[i].getX());
      }
      if(maxRight_.getY() < vertices[i].getY()) {
        maxRight_.setY(vertices[i].getY());
      }
      if(maxRight_.getZ() < vertices[i].getZ()) {
        maxRight_.setZ(vertices[i].getZ());
      }

      // If it is smaller than the current smallest
      if(minLeft_.getX() > vertices[i].getX()) {
        minLeft_.setX(vertices[i].getX());
      }
      if(minLeft_.getY() > vertices[i].getY()) {
        minLeft_.setY(vertices[i].getY());
      }
      if(minLeft_.getZ() > vertices[i].getZ()) {
        minLeft_.setZ(vertices[i].getZ());
      }
    }
  }

}
// EO Namespace