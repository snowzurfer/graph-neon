#ifndef _TEXTURE_COMP_H
#define _TEXTURE_COMP_H

// Includes
#include <lnfw/components/component.h>
#include <windows.h>
#include <gl/GL.h>

namespace winapp {

  class TextureComp : public lnfw::Component 
  {
  public:
    // Register the class
    REGISTER_COMPONENT(TextureComp);

    // Ctor
    TextureComp(GLuint textureID);

    // Getters and setters
    inline GLuint getTextureID() {
      return textureID_;
    }
    inline void setTextureID(GLuint textureID) {
      textureID_ = textureID;
    }

  private:
    // Texture ID
    GLuint textureID_;

  };

}
// EO Namespace

#endif