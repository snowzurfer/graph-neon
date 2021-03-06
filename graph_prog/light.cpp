
// Includes
#include "light.h"
#include <gl/GLU.h>

namespace winapp {

  Light::Light(const int lightNum, const bool isCaster) :
    lightNum_(lightNum), ambient_(kDefaultAmbLight),
    diffuse_(kDefaultDiffLight), specular_(kBaseSpecularLight),
    constAtt_(1.f), linAtt_(0.001f),
    quadAtt_(0.f), enabled_(true) ,
    castsShadow_(isCaster),
    spotCutoff_(180.f)
{
  setPosition(BasePos);
  setSpotDirection(BaseDir);
  apply();
}

void Light::apply() {
  glLightfv(lightNum_, GL_AMBIENT,   ambient_  );
  glLightfv(lightNum_, GL_DIFFUSE,   diffuse_  );
  glLightfv(lightNum_, GL_POSITION,  position_ );
  glLightfv(lightNum_, GL_SPECULAR,  specular_ );
  glLightfv(lightNum_, GL_SPOT_DIRECTION, spotDirection_);

  glLightf (lightNum_, GL_CONSTANT_ATTENUATION,  constAtt_);
  glLightf (lightNum_, GL_LINEAR_ATTENUATION,    linAtt_);
  glLightf (lightNum_, GL_QUADRATIC_ATTENUATION,  quadAtt_);
  glLightf (lightNum_, GL_SPOT_CUTOFF,  spotCutoff_);
  glLightf (lightNum_, GL_SPOT_EXPONENT, 6.0f);

  if(enabled_) {
    glEnable(lightNum_);
  }
  else if(!enabled_) {
    glDisable(lightNum_);
  }
}

void Light::setAmbient(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
  ambient_.r = r;
  ambient_.g = g;
  ambient_.b = b;
  ambient_.a = a;
}

void Light::setAmbient(const GLfloat *params) {
  ambient_ = params;
}

void Light::setSpecular(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
  specular_.r = r;
  specular_.g = g;
  specular_.b = b;
  specular_.a = a;
}

void Light::setSpecular(const GLfloat *params) {
  specular_ = params;
}

void Light::setDiffuse(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
  diffuse_.r = r;
  diffuse_.g = g;
  diffuse_.b = b;
  diffuse_.a = a;
}

void Light::setDiffuse(const GLfloat *params) {
  diffuse_ = params;
}

void Light::setPosition(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) {
  position_[0] = x;
  position_[1] = y;
  position_[2] = z;
  position_[3] = w;
}

void Light::setPosition(const GLfloat *params) {
  setPosition(params[0], params[1], params[2], params[3]);
}

void Light::setSpotDirection(const GLfloat x, const GLfloat y, const GLfloat z) {
  spotDirection_[0] = x;
  spotDirection_[1] = y;
  spotDirection_[2] = z;
}

void Light::setSpotDirection(const GLfloat *params) {
  setSpotDirection(params[0], params[1], params[2]);
}

void Light::draw() {
  glPushAttrib(GL_LIGHTING_BIT);

  glDisable(GL_LIGHTING);

  glTranslatef(position_[0], position_[1], position_[2]);

  glColor3f(1.f, 0.5f, 0.f);
  gluSphere(gluNewQuadric(), 1.f, 6.f, 6.f);
  glColor3f(1.f, 1.f, 1.f);

  glPopAttrib();

}



}
// EO Namespace