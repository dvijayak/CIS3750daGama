#ifndef Color_hpp
#define Color_hpp

#include "global.hpp"

class Color
{
public:
	static const Uint32 Black		= 0x00000000;
	static const Uint32 White		= 0xFFFFFF00;

	static const Uint32 Red 		= 0xFF000000;
	static const Uint32 Green 		= 0x00FF0000;
	static const Uint32 Blue 		= 0x0000FF00;
	static const Uint32 Yellow 	= 0xFFFF0000;
	static const Uint32 Cyan 		= 0x00FFFF00;
	static const Uint32 Pink 		= 0xFF00FF00;

	inline static Uint32 Mix (Uint8 r=0, Uint8 g=0, Uint8 b=0)
	{
		return ( (r & 0xFF) << 24 ) + ( (g & 0xFF) << 16 ) + ( (b & 0xFF) << 8) + 0xFF;
	}
};

#endif