#ifndef IObserver_hpp
#define IObserver_hpp 1

#include "global.hpp"

#include "Subject.hpp"

class IObserver
{
public:
	// Called by the subscribee when a subscribed event is triggered
	// Some values may be passed (old and new) if applicable
	virtual void Notify (Subject*, int, const void*, const void*) = 0;
};

#endif
