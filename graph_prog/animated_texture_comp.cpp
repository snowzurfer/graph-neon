
// Includes
#include <animated_texture_comp.h>
#include <texture_comp.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(AnimatedTextureComp);

  AnimatedTextureComp::AnimatedTextureComp() :
    transform_()
    {

    }

  AnimatedTextureComp::AnimatedTextureComp(const lnfw::Transform<Texel> &transform) :
      transform_(transform) {

      }
  
  AnimatedTextureComp::AnimatedTextureComp(const Texel &velocity, 
    const Texel &angularVelocity, const Texel &scalingVelocity):
    transform_(velocity, angularVelocity, scalingVelocity) {

   }
}
// EO Namespace