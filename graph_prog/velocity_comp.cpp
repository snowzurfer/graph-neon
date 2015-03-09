
// Includes
#include <velocity_comp.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(VelocityComp);

  VelocityComp::VelocityComp() :
    transform_() 
  {

  }

  VelocityComp::VelocityComp(const lnfw::Transform<Vec3> &transform) :
    transform_(transform)
  {

  } 

  VelocityComp::VelocityComp(const Vec3 &vel, const Vec3 &angVel, const Vec3 &scalVel) :
    transform_(vel, angVel, scalVel)
  {

  }
}
// EO Namespace