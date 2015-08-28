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

	// Set up our server TCP socket
	pNetMan->OpenSocket();

	// Begin main loop
	console("Waiting for first connection...");
	size_t count = 0;
	while (true)
	{
		// Listen for incoming connections
		pNetMan->Listen();

		// Read from all clients
		// pNetMan->ReadClients();

		// console(count++);
	}

	Quit();

	return 0;
}
