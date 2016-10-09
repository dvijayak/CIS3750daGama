#ifndef IOBJECT_HPP
#define IOBJECT_HPP

#include "Name.hpp"

class IObject
{
public:
	virtual Name GetName () const = 0;
};

#endif