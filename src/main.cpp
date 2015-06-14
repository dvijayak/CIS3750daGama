#include "global.hpp"

#include "SDLManager.hpp"
#include "NetworkManager.hpp"

void Quit ()
{
	// Destroy managers/singletons and other resources
	SDLManager::Destroy();
}

int main (int argc, char** argv)
{	
	// Initialize SDL. Singleton destruction must be explicitly stated
	SDLManager* pSDLMgr = SDLManager::Instance("Naval Engagements");	
	int init = pSDLMgr->InitializeManagers();	
	if (init)
	{
		Quit();
	}

	NetworkManager* pNetMan = pSDLMgr->GetNetworkManager();
	if (!pNetMan)
	{
		errlog("Couldn't retrieve network manager. No point in continuing...");
		Quit();
	}

	// Listen on a TCP socket for incoming connections and respond when connected
	pNetMan->Listen(5);

	// Say hi?
	console("YO");



	Quit();

	return 0;
}
