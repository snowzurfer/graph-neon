#ifndef _ANIMATED_TEXTURE_COMP_H
#define _ANIMATED_TEXTURE_COMP_H

// Includes
#include <lnfw/components/component.h>
#include <lnfw/physics/transform.h>
#include <texture_comp.h>


namespace winapp {

  class AnimatedTextureComp : public lnfw::Component
  {
  public:
    REGISTER_COMPONENT(AnimatedTextureComp);

    // Default ctor
    AnimatedTextureComp();

    // Ctor
    AnimatedTextureComp(const Texel &velocity,
      const Texel &angularVelocity,  
      const Texel &scalingVelocity);

    // Ctor
    AnimatedTextureComp(const lnfw::Transform<Texel> &transform);

    // Getters and setters
    inline const lnfw::Transform<Texel> &getTransform() const {
      return transform_;
    }
    inline void setTransform(const lnfw::Transform<Texel> &transform) {
      transform_ = transform;
    }
    inline const Texel &getVelocity() const {
      return transform_.position;
    }
    inline const Texel &getAngularVelocity() const {
      return transform_.rotation;
    }
    inline const Texel &getScaleVelocity() const {
      return transform_.scale;
    }

  private:
    // Velocities
    lnfw::Transform<Texel> transform_;

  };
  // EO Class

}
// EO Namespace

#endif