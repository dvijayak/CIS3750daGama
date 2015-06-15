#include "Subject.hpp"

#include "IObserver.hpp"

void Subject::Subscribe (IObserver* pObserver, int event_id)
{
	// Bind the object to the event. Its Notify method will be called
	// upon the emitting of the event
	if (pObserver)
	{
		// First see if the event already has a list of observers
		EventObserverMapType::iterator found = m_observers.find(event_id);
		if (found != m_observers.end())
		{
			// If there are already observers, add this one to the list
			found->second.push_back(pObserver);
		}
		else
		{
			// Otherwise, create a new list and insert into the map
			m_observers[event_id] = std::vector<IObserver*>(1, pObserver);
		}
	}
}

void Subject::Emit (int event_id, const void* new_val, const void* old_val)
{
	// Look for all observers of this event and call Notify on them, passing in any data
	// (usually the old and new values)
	EventObserverMapType::iterator found = m_observers.find(event_id);
	if (found != m_observers.end())
	{
		for (std::vector<IObserver*>::iterator it = found->second.begin();
			it != found->second.end(); ++it)
		{
			if (*it)
			{
				(*it)->Notify(this, event_id, new_val, old_val);
			}
		}
	}
}