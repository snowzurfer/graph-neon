#ifndef _LIGHT_H
#define _LIGHT_H

// Includes
#include <Windows.h>
#include <gl/GL.h>

namespace winapp {

// Define handy light values
const GLfloat WhiteLight[] = {1.f, 1.f, 1.f, 1.f};
const GLfloat BlackLight[] = {0.f, 0.f, 0.f, 0.f};
const GLfloat BaseAmbientLight[] = {0.2f, 0.2f, 0.2f, 1.f};
const GLfloat BaseSpecLight[] = {0.7f, 0.7f, 0.7, 1.f};
const GLfloat BasePos[] = {1.f, 1.f, 1.f, 0.f};

// Define a struct to hold colours for usage in the light class
struct Colour
{
	// RGBA values
	float r,g,b,a;

	// Default ctor
	Colour(const GLfloat *glColour = BlackLight) :
		r(glColour[0]),
		g(glColour[1]),
		b(glColour[2]),
		a(glColour[3]) {};

	// Ctor
	Colour(const GLfloat R, const GLfloat G, const GLfloat B, const GLfloat A) :
		r(R),
		g(G),
		b(B),
		a(A) {};

	// Implicit conversion operator overload
	operator GLfloat* () {
		return (GLfloat*)(this);
	};
};


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

	inline void setShininess(const GLfloat val) {
		shininess_ = val;
	};
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

	// Shininess
	GLfloat shininess_;

	// Specular
	Colour specular_;

	// Position
	GLfloat position_[4];

	// Attenuation
	GLfloat constAtt_, linAtt_, quadAtt_;

	// If light is enabled; DEFAULT == true
	bool enabled_;

	// 
};
// EO Class

}
// EO Namespace


#endif