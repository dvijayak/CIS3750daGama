#ifndef Subject_hpp
#define Subject_hpp 1

#include "global.hpp"

class IObserver;

class Subject
{
public:
	void Subscribe (IObserver* pObserver, int event_id);

	// Emit the event; notify all observers of that event
	void Emit (int event_id, const void* new_val=0, const void* old_val=0);

protected:
	typedef std::map<int, std::vector<IObserver*> > EventObserverMapType;
	EventObserverMapType m_observers;
};

#endif
