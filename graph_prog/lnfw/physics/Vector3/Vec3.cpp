#include "Vec3.h"

 
Vec3::Vec3(float x, float y, float z){
    this->elements[0] = x;
    this->elements[1] = y;
    this->elements[2] = z;
    //std::cout << "Created Vec3 {" << this->elements[0] << ", " << this->elements[1] << ", " << this->elements[2] << "}\n";
}
 
Vec3::~Vec3(){
    //std::cout << "Destroyed Vec3 {" << this->elements[0] << ", " << this->elements[1] << ", " << this->elements[2] << "}\n";
}
 
Vec3 Vec3::copy(){
        Vec3 copy(
                this->elements[0],
                this->elements[1],     
                this->elements[2]
                );
        return copy;           
}
 
bool Vec3::equals(const Vec3& v2, float epsilon) const{
        return ((std::abs(this->elements[0] - v2[0]) < epsilon) &&
                    (std::abs(this->elements[1] - v2[1]) < epsilon) &&
                        (std::abs(this->elements[2] - v2[2]) < epsilon));      
}
 
 
float Vec3::length() const{
        return std::sqrt(this->lengthSquared());
}
 
float Vec3::lengthSquared() const{
        return (
                this->elements[0]*this->elements[0] +
                this->elements[1]*this->elements[1] +
                this->elements[2]*this->elements[2]
                );
}
 
Vec3& Vec3::normalize(){
        float mag = this->length();
        if(mag){
                float multiplier = 1.0f/mag;
                this->elements[0] *= multiplier;
                this->elements[1] *= multiplier;
                this->elements[2] *= multiplier;
        }
        return *this;
}
 
Vec3 Vec3::cross(const Vec3& v2) const{
        Vec3 cross(
                (this->elements[1] * v2[2] - this->elements[2] * v2[1]),
                (this->elements[2] * v2[0] - this->elements[0] * v2[2]),
                (this->elements[0] * v2[1] - this->elements[1] * v2[0])
        );
        return cross;
}
 
Vec3 Vec3::subtract(const Vec3& v1, float scale) const{
        Vec3 sub(
                (this->elements[0] - v1[0]*scale),
                (this->elements[1] - v1[1]*scale),
                (this->elements[2] - v1[2]*scale)
                );
        return sub;
}
 
Vec3& Vec3::set(float x, float y, float z){
        this->elements[0] = x;
        this->elements[1] = y;
        this->elements[2] = z;
        return *this;
}
 
Vec3& Vec3::setX(float x){
        this->elements[0] = x;
        return *this;
}
 
Vec3& Vec3::setY(float y){
        this->elements[1] = y;
        return *this;
}
 
Vec3& Vec3::setZ(float z){
        this->elements[2] = z;
        return *this;
}

float Vec3::getX() const{
       return this->elements[0];
}
 
float Vec3::getY() const{
       return this->elements[1];
}
 
float Vec3::getZ() const{
      return this->elements[2];
}
 
float Vec3::dot(const Vec3& v2) const{
        return (this->elements[0]*v2[0] +
                this->elements[1]*v2[1] +
                this->elements[2]*v2[2]
                );
}
 
Vec3 Vec3::scale(float scale) const{
        Vec3 scaled(
                (this->elements[0]*scale),
                (this->elements[1]*scale),
                (this->elements[2]*scale)
                );
        return scaled;
}
 
Vec3 Vec3::add(const Vec3& v1, float scale) const{
        Vec3 sum(
                (this->elements[0] + v1[0]*scale),
                (this->elements[1] + v1[1]*scale),
                (this->elements[2] + v1[2]*scale)
                );
        return sum;
}
 
 
float Vec3::operator[](int pos) const{
    return elements[pos];
}
 
Vec3 Vec3::operator+(const Vec3& v2) const{
    return this->add(v2);
}
 
Vec3 Vec3::operator-(const Vec3& v2) const{
    return this->subtract(v2);
}
 
Vec3& Vec3::operator+=(const Vec3& v2) {
                this->elements[0] += v2[0];
                this->elements[1] += v2[1];
                this->elements[2] += v2[2];
        return *this;
}
 
Vec3& Vec3::operator-=(const Vec3& v2) {
                this->elements[0] -= v2[0];
                this->elements[1] -= v2[1];
                this->elements[2] -= v2[2];
        return *this;
}

Vec3::operator const float *() {
  return elements;
}

const Vec3 Vec3::operator *(const float rhs) const {
  return this->scale(rhs);
}

const Vec3 Vec3::operator *(const Vec3 &rhs) const {
  Vec3 scaled(
          (this->elements[0]*rhs.getX()),
          (this->elements[1]*rhs.getY()),
          (this->elements[2]*rhs.getZ())
          );
  return scaled;
}

const bool Vec3::operator== (const Vec3 &rhs) const {
  return ( (fabs(this->getX() - rhs.getX()) < EPSILON) && 
    (fabs(this->getY() - rhs.getY()) < EPSILON) &&
    (fabs(this->getZ() - rhs.getZ()) < EPSILON) );
}

const bool Vec3::operator <(const Vec3 &rhs) const {
  return (this->lengthSquared() < rhs.lengthSquared());
}

const bool Vec3::operator >(const Vec3 &rhs) const {
  return (this->lengthSquared() > rhs.lengthSquared());
}
