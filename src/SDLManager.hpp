#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include "global.hpp"

class WindowManager;
class RenderManager;
class NetworkManager;

const int MAX_WIDTH = 800;
const int MAX_HEIGHT = 600;

class SDLManager
{
public:
	static SDLManager* Instance (const std::string& title="Untitled", Uint32 flags=0);
	~SDLManager();
	static void Destroy ();

	int InitializeManagers ();

private:
	SDLManager () {};
	SDLManager (const std::string& title="Untitled", Uint32 flags=0);	
	static SDLManager* s_pInstance;

	std::string m_title;

	// We possess ownership of all managers 
	WindowManager*  m_pWindowManager;
	RenderManager*  m_pRenderManager;
	NetworkManager* m_pNetworkManager;

public:
	inline WindowManager* GetWindowManager () const
	{
		return m_pWindowManager;
	}
	inline RenderManager* GetRenderManager () const
	{
		return m_pRenderManager;
	}
	inline NetworkManager* GetNetworkManager () const
	{
		return m_pNetworkManager;
	}
};

inline void SDLManager::Destroy ()
{
	if (s_pInstance)
	{
		delete s_pInstance;
	}
}

#endif
