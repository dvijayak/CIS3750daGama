#include "NetworkClient.hpp"

#include "NetworkManager.hpp"

size_t NetworkClient::s_suidCount = 0;

void NetworkClient::Notify (Subject* pSubj, int event_id, const void* new_val, const void* old_val)
{
	switch (event_id)
	{
		case NetworkManager::EV_RECVMSSG:
		{
			// Print out the message for now
			if (new_val)
			{
				console("Received message: " << std::string((const char*)new_val));
			}
		}
		break;
	}
}

int NetworkClient::DefaultThreadFunction (void* data)
{
	// Print out the client's name
	if (data)
	{
		NetworkClient* pClient = static_cast<NetworkClient*>(data); // Should always succeed
		console("We are on thread " << SDL_GetThreadID(0) << ". The client's name is " << pClient->GetName());
	}

	return 1;
}

bool NetworkClient::Spawn (NetworkClient::ThreadFunction fn)
{
	// We will pass ourselves to the thread as the data
	m_pThread = SDL_CreateThread(fn, std::string("Client Thread: " + m_name).c_str(), (void*)this);

	if (!m_pThread)
	{
		errlog("Failed to spawn a new thread for the client: " << std::endl << SDL_GetError());
		return false;
	}

	// // We detach the thread as it will be running by itself for a long time
	// // It can interact with clients easily via the NetworkManager singleton
	// // Only thing is we cannot catch return code, so we will have to figure out
	// // a different way to handle errors.
	// SDL_DetachThread(m_pThread);
	int retval;
	SDL_WaitThread(m_pThread, &retval);

	return true;
}