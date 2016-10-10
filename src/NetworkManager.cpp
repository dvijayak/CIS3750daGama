#include "NetworkManager.hpp"

#include <sstream>
#include "NetworkClient.hpp"

NetworkManager::NetworkManager ()
	: m_bFailed(false)
{
	trclog("Initializing network manager...");

	if (SDLNet_Init() == -1)
	{
		m_bFailed = true;
	}

	// Subscribe to self for certain useful events
	Subscribe(this, NetworkManager::EV_NEWCONN);
	Subscribe(this, NetworkManager::EV_RECVMSSG);

	trclog("Network manager initialized.");
}

NetworkManager::~NetworkManager ()
{
	trclog("Destroying network manager...");

	// Not sure if the call to Quit will crash if Init didn't succeed
	if (!m_bFailed)
	{
	 	SDLNet_Quit();
	}

	trclog("Network manager destroyed.");
}

void NetworkManager::Notify (const Subject* pSubj, int event_id, const void* new_val, const void* old_val)
{
	const NetworkClient* pClient = (const NetworkClient*)new_val;

	switch (event_id)
	{
		case NetworkManager::EV_NEWCONN:
		{
			if (pClient)
			{
				console("New connection: "<< pClient->GetName() << " has connected!");
			}
		}
		break;
		// case NetworkClient::EV_DISCONNECT:
		// {
		// 	if (pClient)
		// 	{
		// 		console(pClient->str() << " has disconnected!");
		// 	}

		// 	// TODO: remember client somehow so that upon reconnection, we can re-establish the client
		// }
		// case NetworkClient::EV_QUIT:
		// {
		// 	if (pClient)
		// 	{
		// 		console(pClient->str() << " has quit!");

		// 		// Delete the client
		// 		m_clients.erase(std::find(m_clients.begin(), m_clients.end(), *pClient));
		// 		console("SIZE OF " << m_clients.size());
		// 	}
		// }
	}
}

bool NetworkManager::OpenSocket ()
{
	// Open up a TCP socket	
	if (SDLNet_ResolveHost(&m_ip, 0, PORT) == -1)
	{
		errlog("Couldn't resolve host: " << std::endl << SDLNet_GetError());
		return false;
	}	
	m_serversock = SDLNet_TCP_Open(&m_ip);
	if (!m_serversock)
	{
		errlog("Couldn't open server socket: " << std::endl << SDLNet_GetError());
		return false;
	}

	return true;
}

bool NetworkManager::Listen ()
{
	// Listen for an incoming connection
	TCPsocket newconn = SDLNet_TCP_Accept(m_serversock);
	if (!newconn)
	{
		return false;
	}

	console("Connection established!");

	// Spawn a new client and let the world know that it has connected!
	Emit(NetworkManager::EV_NEWCONN, (void*)&(SpawnClient(newconn)));

	return true;
}

NetworkClient& NetworkManager::SpawnClient (TCPsocket& newconn)
{
	std::stringstream client_name;
	client_name << "Client" << m_clients.size()+1;
	m_clients.push_back(NetworkClient(newconn, client_name.str()));

	// We use the object from the clients list so that the reference is valid
	// for the entire duration of the manager object. Recall that the std::vector
	// takes ownership of all its elements and manages their lifecycle.
	NetworkClient& newclient = m_clients.back();

	// IMPORTANT:
	// The statement m_clients.push_back(NetworkClient(newconn, client_name.str()));
	// will do the following:
	//		1. Create a new NetworkClient object.
	//		2. Pass a copy of this object to push_back.
	// Step 2 results in the invoking of the constructor of the copy
	// AS WELL AS THE destructor of the original object when the calling
	// method (in this case, NetworkManager::SpawnClient) returns, since
	// the original will go out of scope. Thus, it was decided to move away
	// thread spawning and socket closing from the constructor and destructor
	// and perform them explicitly/independently when needed.

	// Spawn a new thread for the client
	newclient.Spawn();

	// Subscribe to the client (and vice versa, if necessary) for various events
	newclient.Subscribe(this, NetworkClient::EV_DISCONNECT);
	newclient.Subscribe(this, NetworkClient::EV_QUIT);

	return newclient;
}

void NetworkManager::ReadClients ()
{
	// Go through all clients and read from their sockets
	for (const NetworkClient& client : m_clients)
	{
		std::string message;
		bool result = ReadMessage(client.GetSocket(), message);
		if (result)
		{
			// Successful read; let observers know
			Emit(NetworkManager::EV_RECVMSSG, (void*)message.c_str());
		}
	}
}

bool NetworkManager::Read (const TCPsocket& sock, char* data, size_t n) const
{	
	// console("READ1");
	int result = SDLNet_TCP_Recv(sock, (void*)data, n);
	// console("\tREAD2");
	if (result <= 0)
	{
		errlog("Error reading from socket: " << std::endl << SDLNet_GetError());
		return false;
	}

	return true;
}

bool NetworkManager::Write (const TCPsocket& sock, const char* data, size_t n) const
{
	size_t result = SDLNet_TCP_Send(sock, (const void*)data, n);
	if (result < n)
	{
		errlog("Error writing to socket: " << std::endl << SDLNet_GetError() << std::endl << "(We should probably close the socket)");
		return false;
	}

	return true;
}

bool NetworkManager::SendMessage (const TCPsocket& sock, MessageName name, size_t n_args, ...) const
{
	if (name.str() == "")
	{
		errlog("Error writing to socket: Invalid message name.");
		return false;
	}

	// Construct message args from the input
	std::string message;
	va_list args;
	va_start(args, n_args);
	for (size_t i = 0; i < n_args; ++i)
	{
		// ATTN: string concatenation is expensive!		
		// ATTN: var arg string is passed by value instead of reference!
		message += std::string(va_arg(args, char*));

		// End the message with newline
		// TODO: should be a class constant
		if (i == n_args-1)
		{
			message += '\n';
		}
		// Or add normal delimiter
		// TODO: should be a class constant
		else
		{
			message += ":";
		}
	}
	va_end(args);

	return Write(sock, std::string(name.str() + ":" + message).c_str(), message.size());
}

bool NetworkManager::ReadMessage (const TCPsocket& sock, std::string& message) const
{
	// Keep reading a byte until we reach a newline
	// TODO: should be a class constant
	std::vector<char> buffer;
	while (true)
	{
		// Read the next byte
		char byte;
		bool result = Read(sock, &byte, 1);
		if (!result)
		{
			return false;
		}

		// If it is a newline, break
		if (byte == '\n')
		{
			break;
		}
		// Else, append to buffer
		else
		{
			buffer.push_back(byte);
		}
	}

	// Convert byte buffer to string
	message = std::string(buffer.begin(), buffer.end());

	return true;
}