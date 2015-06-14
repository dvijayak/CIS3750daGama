#ifndef NetworkManager_hpp
#define NetworkManager_hpp 1

#include "global.hpp"

#include <cstdarg>

class SDLManager;
class NetworkClient;

class NetworkManager
{
public:
	// This class encapsulates the "name" or "function" part of a message
	// to/from a client and thus enforces conformance to the specification.
	class MessageName
	{
	public:
		enum
		{
			REGISTER,
			READY,
			SHORE,
			START,
			FIRE
		};

		MessageName (int value)
			: m_value(value)
		{}

		std::string str ()
		{
			switch (m_value)
			{
				case MessageName::REGISTER:
					return "register";
				case MessageName::READY:
					return "ready";
				case MessageName::SHORE:
					return "shore";
				case MessageName::START:
					return "start";
				case MessageName::FIRE:
					return "fire";
			}

			// Invalid message
			return "";
		}

	private:
		int m_value;
	};
	
	~NetworkManager ();

	// Listen to the next n incoming connections and spawn a new client.
	// If n is 0, then we listen for an infinite number of connections (probably a
	// bad idea).
	// Note: this function is BLOCKING
	bool Listen (size_t n=0);

	// Read the specified amount of raw bytes from the given socket
	// Caller is responsible to ensure that data can hold n bytes
	bool Read (TCPsocket& sock, char* data, size_t n) const;

	// Write the specified amount of raw bytes to the given socket
	bool Write (TCPsocket& sock, const char* data, size_t n) const;

	// Send a specification-compliant message to the given client
	bool SendMessage (TCPsocket& sock, MessageName name, size_t n_args, ...) const;

	// Read a newline-terminated (as per spec) message from the given client
	bool ReadMessage (TCPsocket& sock, std::string& message) const;

public:
	static const size_t PORT = 5283;

private:
	// Force singleton usage
	friend SDLManager;
	NetworkManager ();

	// Server IP address
	IPaddress m_ip;

	// Server socket
	TCPsocket m_serversock;

	// Successfully connected clients
	std::vector<NetworkClient> m_clients;

	// Did we fail to initialize SDL_Net?
	bool m_bFailed;

public:
	inline const std::vector<NetworkClient>& GetClients () const
	{
		return m_clients;
	}

	inline bool Failed () const
	{
		return m_bFailed;
	}
};

#endif
