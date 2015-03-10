#ifndef _VELOCITY_COMP_H
#define _VELOCITY_COMP_H


// Includes
#include <lnfw/components/component.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <lnfw/physics/transform.h>

namespace winapp {

  class VelocityComp : public lnfw::Component
  {
  public:
    REGISTER_COMPONENT(VelocityComp);

    // Default ctor
    VelocityComp();

    // Parameterised ctor
    VelocityComp(const Vec3 &vel, const Vec3 &angVel, const Vec3 &scalVel);
    VelocityComp(const lnfw::Transform<Vec3> &transform);

    // Getters and setters
    inline const lnfw::Transform<Vec3> &getTransform() const {
      return transform_;
    }
    inline void setTransform(const lnfw::Transform<Vec3> &transform) {
      transform_ = transform;
    }
    inline const Vec3 &getVelocity() const {
      return transform_.position;
    }
    inline const Vec3 &getAngularVelocity() const {
      return transform_.rotation;
    }
    inline const Vec3 &getScaleVelocity() const {
      return transform_.scale;
    }

  private:
    // Velocities
    lnfw::Transform<Vec3> transform_;
    
  };
  // EO Class

}
// EO Namespace

#endif