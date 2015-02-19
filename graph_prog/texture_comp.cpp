
// Includes
#include <texture_comp.h>

namespace winapp {

  REGISTER_COMPONENT_IMPL(TextureComp);

  TextureComp::TextureComp(GLuint textureID) :
    textureID_(textureID)
  {

  }

}
// EO Namespace