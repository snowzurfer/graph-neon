
// Includes
#include <texture_comp.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(TextureComp);

  TextureComp::TextureComp(GLuint textureID) :
    textureID_(textureID)
  {

  }


  const Texel operator *(const Texel &lhs, const float rhs) {
    return Texel(lhs.x * rhs, lhs.y * rhs);
  }

  /*const Texel Texel::operator *(const float rhs) const {
    return Texel(this->x * rhs, this->y * rhs);
  }

  const Texel Texel::operator +(const Texel &rhs) const {
    return Texel(this->x + rhs.x, this->y + rhs.y);
  }*/

  const Texel operator +(const Texel &lhs, const Texel &rhs) {
    return Texel(lhs.x + rhs.x, lhs.y + rhs.y);
  }

}
// EO Namespace