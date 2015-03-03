#ifndef _TRANSFORM_H
#define _TRANSFORM_H


namespace lnfw {
  
  template <typename V>
  class Transform
  {
  public:
	  // Ctor
	  Transform() :
      position(),
      scale(),
      rotation() {};

    Transform(const V &positionE,
      const V &rotationE,  
      const V &scaleE):
      position(positionE),
      scale(scaleE),
      rotation(rotationE) {};

	  // Components of the transform
	  V position;
	  V scale;
	  V rotation;

    
    
    
    const Transform<V> operator +(const Transform<V> &rhs) const {
      return Transform(position + rhs.position,
        rotation + rhs.rotation,
        scale + rhs.scale);
    }
    
    const Transform<V> operator *(const Transform<V> &rhs) const {
      return Transform(position * rhs.position,
        rotation * rhs.rotation,
        scale * rhs.scale);
    }
    
    const Transform<V> operator *(const float rhs) const {
      return Transform(position * rhs,
        rotation * rhs,
        scale * rhs);
    }

  };
  // EO Class


  // Template specialisation
  /*template<>
  class Transform<Vec3>
  {
    // Ctor
    Transform();

    Transform(const Vec3 &positionE,
      const Vec3 &rotationE,  
      const Vec3 &scaleE);

    // Components of the transform
    Vec3 position;
    Vec3 scale;
    Vec3 rotation;

    friend const Transform<Vec3> operator +(const Transform<Vec3> &lhs,
      const Transform<Vec3> &rhs);
    friend const Transform<Vec3> operator *(const Transform<Vec3> &lhs,
      const Transform<Vec3> &rhs);
    friend const Transform<Vec3> operator *(const Transform<Vec3> &lhs,
      const float &rhs);
  };


  using winapp::Texel;

  // Template specialisation
  template<>
  class Transform<Texel>
  {
    // Ctor
    Transform();

    Transform(const Texel &positionE,
      const Texel &rotationE,  
      const Texel &scaleE);

    // Components of the transform
    Texel position;
    Texel scale;
    Texel rotation;

    friend const Transform<Texel> operator +(const Transform<Texel> &lhs,
      const Transform<Texel> &rhs);
    friend const Transform<Texel> operator *(const Transform<Texel> &lhs,
      const Transform<Texel> &rhs);
    friend const Transform<Texel> operator *(const Transform<Texel> &lhs,
      const float &rhs);
  };*/

}
// EO Namespace



#endif