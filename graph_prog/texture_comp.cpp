
// Includes
#include <texture_comp.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(TextureComp);

  TextureComp::TextureComp(GLuint textureID) :
    textureID_(textureID)
  {

  }


  Texel operator *(const Texel &lhs, const float rhs) {
    return Texel(lhs.x * rhs, lhs.y * rhs);
  }
}
// EO Namespace