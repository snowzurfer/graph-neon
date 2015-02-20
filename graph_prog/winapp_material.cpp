
// Includes
#include "winapp_material.h"

namespace winapp {

  REGISTER_COMPONENT_IMPL(MaterialComp)

  MaterialComp::MaterialComp() :
  Component(),
    ambient_(kDefaultAmbMat),
    diffuse_(kDefaultDiffMat),
    specular_(kDefaultSpecMat),
    shininess_(kNoShininess),
    emission_(kBlackColourNoAlpha)
  {

  }

  void MaterialComp::apply(uint32_t face) const {
    glMaterialf (face, GL_SHININESS, shininess_);
    glMaterialfv(face, GL_AMBIENT,   ambient_  );
    glMaterialfv(face, GL_DIFFUSE,   diffuse_  );
    glMaterialfv(face, GL_EMISSION,  emission_);
    glMaterialfv(face, GL_SPECULAR,  specular_ );
  }

  void MaterialComp::setAmbient(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    ambient_.r = r;
    ambient_.g = g;
    ambient_.b = b;
    ambient_.a = a;
  }

  void MaterialComp::setAmbient(const GLfloat *params) {
    ambient_ = params;
  }

  void MaterialComp::setSpecular(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    specular_.r = r;
    specular_.g = g;
    specular_.b = b;
    specular_.a = a;
  }

  void MaterialComp::setSpecular(const GLfloat *params) {
    specular_ = params;
  }

  void MaterialComp::setDiffuse(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    diffuse_.r = r;
    diffuse_.g = g;
    diffuse_.b = b;
    diffuse_.a = a;
  }

  void MaterialComp::setDiffuse(const GLfloat *params) {
    diffuse_ = params;
  }

  void MaterialComp::setEmission(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    emission_.r = r;
    emission_.g = g;
    emission_.b = b;
    emission_.a = a;
  }

  void MaterialComp::setEmission(const GLfloat *params) {
    emission_ = params;
  }
}
// EO Namespace