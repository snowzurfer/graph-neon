#ifndef _TEXTURE_COMP_H
#define _TEXTURE_COMP_H

// Includes
#include <lnfw/components/component.h>
#include <lnfw/physics/transform.h>
#include <windows.h>
#include <gl/GL.h>

namespace winapp {

  class Texel 
  {
  public:
    Texel():
        x(0.f), y(0.f) {};

    Texel(const float xCoord, const float yCoord) :
      x(xCoord), y(yCoord) {};

    float x, y;

    friend Texel operator *(const Texel &lhs, const float rhs);
  };

  class TextureComp : public lnfw::Component 
  {
  public:
    // Register the component
    REGISTER_COMPONENT(TextureComp);

    // Ctor
    TextureComp(GLuint textureID);

    // Getters and setters
    inline const GLuint getTextureID() const {
      return textureID_;
    }
    inline void setTextureID(GLuint textureID) {
      textureID_ = textureID;
    }
    inline const lnfw::Transform<Texel> &getTransform() const {
      return transform_;
    }
    inline void setTransform(const lnfw::Transform<Texel> &transform) {
      transform_ = transform;
    }
    inline const Texel &getPosition() const {
      return transform_.position;
    }
    inline const Texel &getRotation() const {
      return transform_.rotation;
    }
    inline const Texel &getScale() const {
      return transform_.scale;
    }

  private:
    // Texture ID
    GLuint textureID_;

    // Transform
    lnfw::Transform<Texel> transform_;
  };

}
// EO Namespace

#endif