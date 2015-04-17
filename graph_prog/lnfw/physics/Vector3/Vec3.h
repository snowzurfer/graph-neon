#ifndef _VEC3_H_
#define _VEC3_H_

#include <cmath>
#include <iostream>
 
class Vec3{
private:
    float elements[3];
public:
    Vec3(float x = 0, float y = 0, float z = 0);
    ~Vec3();
    Vec3 copy();
 
 
    Vec3& set(float x, float y, float z);
    Vec3& setX(float x);
    Vec3& setY(float y);
    Vec3& setZ(float z);

  float getX() const;
  float getY() const;
  float getZ() const;
 
    Vec3 add(const Vec3& v1, float scale=1.0) const;
    Vec3 subtract(const Vec3& v1, float scale=1.0) const;
    Vec3 scale(float scale) const;
 
    float dot(const Vec3& v2) const;
    Vec3 cross(const Vec3& v2) const;
 
    Vec3& normalize();
    float length() const;
    float lengthSquared() const;
 
    bool equals(const Vec3& v2, float epsilon) const;
    float operator[](int pos) const;
 
    Vec3 operator+(const Vec3& v2) const;
    Vec3 operator-(const Vec3& v2) const;
 
    Vec3& operator+=(const Vec3& v2);
    Vec3& operator-=(const Vec3& v2);

    operator const float *();

    // Modified by Alberto Taiuti
    const Vec3 operator *(const Vec3 &rhs) const;
    const Vec3 operator *(const float rhs) const;
    const bool operator ==(const Vec3 &rhs) const;
    const bool operator <(const Vec3 &rhs) const;
    const bool operator >(const Vec3 &rhs) const;
};

#endif