
// Includes
#include <lnfw/physics/transform.h>

namespace lnfw {


  template <typename V>
  const Transform<V> operator +(const Transform<V> &lhs,
    const Transform<V> &rhs) {
    return Transform(lhs.position + rhs.position,
      lhs.rotation + rhs.rotation,
      lhs.scale + rhs.scale);
  }

  template <typename V>
  const Transform<V> operator *(const Transform<V> &lhs,
    const Transform<V> &rhs) {
      return Transform(lhs.position * rhs.position,
        lhs.rotation * rhs.rotation,
        lhs.scale * rhs.scale);
  }

  template <typename V>
  const Transform<V> operator *(const Transform<V> &lhs,
    const float rhs) {
      return Transform(lhs.position * rhs,
        lhs.rotation * rhs,
        lhs.scale * rhs);
  }

  template <typename V>
  const Transform<V> Transform<V>::operator +(const Transform<V> &rhs) const {
      return Transform(this->position + rhs.position,
        this->rotation + rhs.rotation,
        this->scale + rhs.scale);
  }

  template <typename V>
  const Transform<V> Transform<V>::operator *(const Transform<V> &rhs) const {
      return Transform(this->position * rhs.position,
        this->rotation * rhs.rotation,
        this->scale * rhs.scale);
  }

  template <typename V>
  const Transform<V> Transform<V>::operator *(const float rhs) const {
      return Transform(this->position * rhs,
        this->rotation * rhs,
        this->scale * rhs);
  }
}
// EO Namespace