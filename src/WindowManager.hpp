#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "global.hpp"

class WindowManager
{
public:
	WindowManager () {};
	WindowManager (std::string title, int width, int height, Uint32 flags = 0);
	~WindowManager ();

	SDL_Window* GetWindow () const;

	std::string GetTitle () const;
	int GetWidth () const;
	int GetHeight () const;

private:
	SDL_Window* m_pWindow;
};

#endif
