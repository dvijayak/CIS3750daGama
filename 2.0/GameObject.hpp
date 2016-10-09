#ifndef GO_HPP
#define GO_HPP

#include <map>

#include "IObject.hpp"

class GameObject
	: IObject
{
public:
	GameObject ();

	/// IObject
	Name GetName () const { return m_name;	}

	typedef std::map<IComponent*> ComponentContainer
	ComponentContainer const& GetComponents () const { return m_components; }
	template <class T> IComponent* GetComponent ();

	bool AddComponent (IComponent const* pComp);
	bool RemoveComponent (IComponent const* pComp);
	template <class T> void RemoveComponent ();

private:
	Name m_name;

	// Components are indexed in the container by their "type id"
	ComponentContainer m_components;
};

template <class T> IComponent* GetComponent ()
{
	ComponentContainer::iterator it = m_components.find(T::CID);
	if (it != m_components.end())
	{
		return *it;
	}

	return 0;
}

template <class T> bool RemoveComponent ()
{
	ComponentContainer::iterator it = m_components.find(T::CID);
	if (it != m_components.end())
	{
		m_components.erase(it);
		return true;
	}

	return false;
}

#endif