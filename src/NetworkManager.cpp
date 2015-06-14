#include "NetworkManager.hpp"

#include "NetworkClient.hpp"

NetworkManager::NetworkManager ()
	: m_bFailed(false)
{
	trclog("Initializing network manager...");

	if (SDLNet_Init() == -1)
	{
		m_bFailed = true;
	}

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

bool NetworkManager::Listen (size_t n)
{
	// Open up a TCP socket
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, 0, PORT) == -1)
	{
		errlog("Couldn't resolve host: " << std::endl << SDLNet_GetError());
		return false;
	}
	TCPsocket serversock = SDLNet_TCP_Open(&ip);
	if (!serversock)
	{
		errlog("Couldn't open server socket: " << std::endl << SDLNet_GetError());
		return false;
	}

	// Listen for n (or infinite if n is 0) connections
	for (size_t i = 0; i < n && n != 0; ++i)
	{
		// (Begin BLOCKING)
		console("Waiting for connection...");
		TCPsocket newconn;
		do
		{
			newconn = SDLNet_TCP_Accept(serversock);
		} while (!newconn);
		// (End BLOCKING)

		console("\tConnection established!");

		// Spawn a new client
		m_clients.push_back(NetworkClient(newconn, "client"));
	}

	return true;
}

bool NetworkManager::Read (TCPsocket& sock, char* data, size_t n) const
{	
	int result = SDLNet_TCP_Recv(sock, (void*)data, n);
	if (result <= 0)
	{
		errlog("Error reading from socket: " << std::endl << SDLNet_GetError());
		return false;
	}

	return true;
}

bool NetworkManager::Write (TCPsocket& sock, const char* data, size_t n) const
{
	int result = SDLNet_TCP_Send(sock, (void*)data, n);
	if (result < n)
	{
		errlog("Error writing to socket: " << std::endl << SDLNet_GetError() << std::endl << "(We should probably close the socket)");
		return false;
	}

	return true;
}

bool NetworkManager::SendMessage (TCPsocket& sock, MessageName name, size_t n_args, ...) const
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

bool NetworkManager::ReadMessage (TCPsocket& sock, std::string& message) const
{
	return true;
}