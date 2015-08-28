#include "NetworkClient.hpp"

#include "SDLManager.hpp"
#include "NetworkManager.hpp"

size_t NetworkClient::s_suidCount = 0;

NetworkClient::~NetworkClient ()
{
	// TODO: Perform cleanup as necessary EXCEPT FOR SOCKET CLOSING
}

void NetworkClient::Notify (const Subject* pSubj, int event_id, const void* new_val, const void* old_val)
{
	// switch (event_id)
	// {
	// 	case NetworkManager::EV_RECVMSSG:
	// 	{
	// 		// Print out the message for now
	// 		if (new_val)
	// 		{
	// 			console("Received message: " << std::string((const char*)new_val));
	// 		}
	// 	}
	// 	break;
	// }
}

void NetworkClient::Initialize ()
{
	// TODO
}

int NetworkClient::DefaultThreadFunction (void* data)
{
	NetworkClient* pClient = static_cast<NetworkClient*>(data); // Should always succeed
	if (!pClient)
	{
		errlog("Couldn't retrieve NetworkClient even though that is what was passed.");
		return 0;
	}

	NetworkManager* pNM = SDLManager::Instance()->GetNetworkManager();
	if (!pNM)
	{
		errlog("Couldn't retrieve NetworkManager.");
		return 0;
	}

	// Keep listening for and reacting to messages from the socket
	bool bError = false;
	std::string message;
	console(pClient->GetName() << "[Thread id " << SDL_GetThreadID(0) << "] is listening for messages now...");
	while (true)
	{
		// Go through all clients and read from their sockets
		bool result = pNM->ReadMessage(pClient->GetSocket(), message);
		if (result)
		{
			// TODO: Print out the message for now
			console("Received message from " << pClient->str() << ": " << message);

			// Successful read; let observers know
			// pClient->Emit(NetworkManager::EV_RECVMSSG, (void*)message.c_str());

			message.clear();
		}
		else
		{
			// An error occurred reading; most likely a socket error, so just quit for now
			bError = true;
			errlog("An error occurred while reading. Most likely a socket error so just disconnect for now.");

			// TODO: Inform Network Manager that client has disconnected
			// pClient->Emit(NetworkClient::EV_DISCONNECT, (void*)this);

			break;
		}
	}

	// Close the socket
	SDLNet_TCP_Close(pClient->GetSocket());

	// TODO: Inform Network Manager that client has quit
	// pClient->Emit(NetworkClient::EV_QUIT, (void*)this);

	return (int)!bError;
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

	// We detach the thread as it will be running by itself for a long time
	// It can interact with clients easily via the NetworkManager singleton
	// Only thing is we cannot catch return code, so we will have to figure out
	// a different way to handle errors.
	SDL_DetachThread(m_pThread);

	return true;
}