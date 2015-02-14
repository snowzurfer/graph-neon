
// Includes
#include "winapp_material.h"

namespace winapp {

  Material::Material() :
  ambient_(kDefaultAmbMat),
  diffuse_(kDefaultDiffMat),
  specular_(kDefaultSpecMat),
  shininess_(kNoShininess),
  emission_(kBlackColourNoAlpha)
{

}

  void Material::apply(uint32_t face) {
    glMaterialf (face, GL_SHININESS, shininess_);
    glMaterialfv(face, GL_AMBIENT,   ambient_  );
    glMaterialfv(face, GL_DIFFUSE,   diffuse_  );
    glMaterialfv(face, GL_EMISSION,  emission_);
    glMaterialfv(face, GL_SPECULAR,  specular_ );
  }

  void Material::setAmbient(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    ambient_.r = r;
    ambient_.g = g;
    ambient_.b = b;
    ambient_.a = a;
  }

  void Material::setAmbient(const GLfloat *params) {
    ambient_ = params;
  }

  void Material::setSpecular(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    specular_.r = r;
    specular_.g = g;
    specular_.b = b;
    specular_.a = a;
  }

  void Material::setSpecular(const GLfloat *params) {
    specular_ = params;
  }

  void Material::setDiffuse(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    diffuse_.r = r;
    diffuse_.g = g;
    diffuse_.b = b;
    diffuse_.a = a;
  }

  void Material::setDiffuse(const GLfloat *params) {
    diffuse_ = params;
  }

  void Material::setEmission(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
    emission_.r = r;
    emission_.g = g;
    emission_.b = b;
    emission_.a = a;
  }

  void Material::setEmission(const GLfloat *params) {
    emission_ = params;
  }
}
// EO Namespace