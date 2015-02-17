
// Includes
#include "subject.h"

namespace lnfw {

Subject::Subject() :
	observers_() {
}

void Subject::removeObserver(ObserverComponent *observer) {
	// Find if the observer being removed is actually present
	observersItor itorObs = 
		std::find(observers_.begin(), observers_.end(), observer);
	
	// If the observer to be removed has been found
	if(itorObs != observers_.end()) {
		// Remove it from the list
		observers_.erase(itorObs);
	}
}

void Subject::addObserver(ObserverComponent *observer) {
	// If the pointer is valid
	if(observer != NULL) {
		// Add the observer to the list
		observers_.push_back(observer);

		// Inform the observer of its reference system
		observer->addSubjectReference(this);
	}
}

void Subject::sendMessage(lnfw::Message *message) const{
	// For each attached observer
	for(constObserversItor itor = observers_.begin(); itor != observers_.end(); ++itor) {
		// Send the message to it
		(*itor)->onNotify(message);
	}
}

}
// EO Namespace