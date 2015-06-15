#ifndef NetworkManager_hpp
#define NetworkManager_hpp 1

#include "global.hpp"

#include <cstdarg>
#include "Subject.hpp"
#include "IObserver.hpp"

class SDLManager;
class NetworkClient;

class NetworkManager
	: public Subject
	, public IObserver
{
public:
	enum EventEnum
	{
		EV_NEWCONN, // a new client has connected
		EV_RECVMSSG, // a new message has been received
	};

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
	
	virtual ~NetworkManager ();

	void Notify (Subject*, int, const void*, const void*);

	// Open a TCP server socket
	bool OpenSocket ();

	// Listen to an incoming connection and respond.
	bool Listen ();

	// Spawn a new client
	NetworkClient& SpawnClient (TCPsocket& newconn);

	// Check all client sockets for any input
	void ReadClients ();

	// Read the specified amount of raw bytes from the given socket
	// Caller is responsible to ensure that data can hold n bytes
	bool Read (const TCPsocket& sock, char* data, size_t n) const;

	// Write the specified amount of raw bytes to the given socket
	bool Write (const TCPsocket& sock, const char* data, size_t n) const;

	// Send a specification-compliant message to the given client
	bool SendMessage (const TCPsocket& sock, MessageName name, size_t n_args, ...) const;

	// Read a newline-terminated (as per spec) message from the given client
	bool ReadMessage (const TCPsocket& sock, std::string& message) const;

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
