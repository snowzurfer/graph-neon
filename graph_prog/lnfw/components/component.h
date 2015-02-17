#ifndef	_COMPONENT_H
#define _COMPONENT_H


// Includes


namespace lnfw {

//#define REGISTER_COMPONENT(component_name) \
//	static Int32 ##component_name_type;
//	Int32 GetComponentType() { return #component_name#_type_id_; }

//#define REGISTER_COMPONENT_IMPL(component_name, component_name_string) \
//	Int32 #component_name#_type_id_ = abfw::CRC::GetICRC(component_name_string);

	//static const Int32 GetComponentTypeID() { return #component_name#_type_id_; } \

class Component 
{
public:
	// Ctor
	Component(const int type) :
		type_(type) {}

	// Dtor
	virtual ~Component() {};

	// Getters and setters
	inline const int getType() const { return type_; };
	inline void setType(const int v) { type_ = v; }; // NOTE: Should have assert!

protected:
	// Type of the component
	int type_;
};
// EO Class

}
// EO Namespace


#endif