#ifndef _LIGHT_H
#define _LIGHT_H

// Includes
#include <Windows.h>
#include <gl/GL.h>
#include "winapp_colour.h"

namespace winapp {

// Define handy colour values
const Colour kBaseSpecularLight(0.8f, 0.8f, 0.8f, 1.f);
const GLfloat BasePos[] = {1.f, 1.f, 1.f, 0.f};

class Light 
{
public:
  // Ctor
  // Pass the number of the light to be used
  Light(const int lightNum);

  // Getters and setters
  void setAmbient(const GLfloat *params);
  void setAmbient(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  void setDiffuse(const GLfloat *params);
  void setDiffuse(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  void setSpecular(const GLfloat *params);
  void setSpecular(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  void setPosition(const GLfloat *params);
  void setPosition(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);

  
  inline void setConstAttenuation(const GLfloat val) {
    constAtt_ = val;
  };
  inline void setLinAttenuation(const GLfloat val) {
    linAtt_ = val;
  };
  inline void setQuadAttenuation(const GLfloat val) {
    quadAtt_ = val;
  };
  
  
  inline const bool isEnabled() const {
    return enabled_;
  };
  inline void setEnabled(const bool enabled) {
    enabled_ = enabled;
  };
  inline const int getLightNum() const {
    return lightNum_;
  };

  // Apply the members of the class to the GL state machine
  void apply();

private:
  // Light number
  int lightNum_;

  // Ambient
  Colour ambient_;

  // Diffuse
  Colour diffuse_;

  // Specular
  Colour specular_;

  // Position, with w value
  GLfloat position_[4];

  // Attenuations
  GLfloat constAtt_, linAtt_, quadAtt_;

  // If light is enabled; DEFAULT == true
  bool enabled_;

};
// EO Class

}
// EO Namespace

#endif