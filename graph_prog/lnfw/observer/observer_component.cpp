
// Includes
#include "observer_component.h"
#include "subject.h"

namespace lnfw {

ObserverComponent::ObserverComponent(const int type) :
	Component(type),
	subjectReference_(NULL) {
}

ObserverComponent::~ObserverComponent() {
	// Report to system of deletion
	subjectReference_->removeObserver(this);
}

void ObserverComponent::addSubjectReference(Subject *subjectReference) {
	if(subjectReference != NULL) {
		subjectReference_ = subjectReference;
	}
}

}
// EO Namespace