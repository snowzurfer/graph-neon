#ifndef	_COMPONENT_H
#define _COMPONENT_H


// Includes
#include <tools/crc.h>

namespace lnfw {

#define REGISTER_COMPONENT(componentClassName) \
	static unsigned int componentClassName##TypeID; \
	unsigned int GetComponentType() { return componentClassName##TypeID; }

#define REGISTER_COMPONENT_IMPL(componentClassName) \
	unsigned int componentClassName::componentClassName##TypeID = abfw::CRC::GetICRC(#componentClassName);

class Component 
{
public:
	// Ctor
  Component() {};

	// Dtor
	virtual ~Component() {};

  virtual unsigned int GetComponentType() = 0;

};
// EO Class

}
// EO Namespace


#endif