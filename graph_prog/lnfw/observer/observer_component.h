#ifndef _OBSERVER_COMPONENT_H
#define _OBSERVER_COMPONENT_H


// Includes
#include <lnfw\components\component.h>
#include <lnfw\observer\message.h>


// Forward declarations
namespace lnfw {
	class Subject;
}


namespace lnfw {

class ObserverComponent : public Component
{
public:

  REGISTER_COMPONENT(ObserverComponent);

	// Ctor
	ObserverComponent (const int type);

	// Dtor
	virtual ~ObserverComponent();

	// Handle received message in a way defined specifically by child classes
	virtual void onNotify(lnfw::Message *message) {};

	// Add the reference to the subject.
	// If already available, the function will override
	// the previous one.
	void addSubjectReference(Subject *subjectReference);

protected:
	// Reference to the subject. Used to un-register
	Subject *subjectReference_;

};
// EO Class

}
// EO Namespace


#endif