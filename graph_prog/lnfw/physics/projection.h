#ifndef _PROJECTION_H
#define _PROJECTION_H


namespace lnfw {

class Projection
{
private:
	// Minimum value of the projection on a given axis
	float min_;

	// Maximum value of the projection on a given axis
	float max_;
public:
	// Ctor
	Projection(float min, float max) : 
		min_(min), max_(max) {};

	// Getters
	inline const float getMin() const { return min_; };
	inline const float getMax() const { return max_; };
};
// EO Class

}
// EO NamespaceQuickScope

#endif