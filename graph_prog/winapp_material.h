#ifndef _WINAPP_MATERIAL_H
#define _WINAPP_MATERIAL_H

#include <windows.h>
#include <gl/GL.h>
#include <winapp_colour.h>
#include <cstdint>
#include <lnfw/components/component.h>

namespace winapp {


// Handy material values
const Colour kNoMaterial(0.f, 0.f, 0.f, 1.f);
const Colour kDefaultAmbMat(0.7f, 0.7f, 0.7f, 1.f);
const Colour kDefaultDiffMat(0.85f, 0.85f, 0.85f, 1.f);
const Colour kDefaultSpecMat(1.f, 1.f, 1.f, 1.f);
const GLfloat kNoShininess = 0.f;
const GLfloat kLowShininess = 0.5;
const GLfloat kHighShininess = 100.f;

class MaterialComp : public lnfw::Component
{
public:
  // Register the component
  REGISTER_COMPONENT(MaterialComp);

  // Ctor
  MaterialComp();

  // Apply the members of the class to the GL state machine
  // on the specified face.
  void apply(uint32_t face) const;

  // Getters and setters
  void setAmbient(const GLfloat *params);
  void setAmbient(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  void setDiffuse(const GLfloat *params);
  void setDiffuse(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  void setSpecular(const GLfloat *params);
  void setSpecular(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  void setEmission(const GLfloat *params);
  void setEmission(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
  inline void setShininess(const GLfloat val) {
    shininess_ = val;
  };

private:
  // Ambient
  Colour ambient_;

  // Diffuse
  Colour diffuse_;

  // Specular
  Colour specular_;

  // Shininess
  GLfloat shininess_;

  // Emissive colour of material
  Colour emission_;

};


}
// EO Namespace

#endif