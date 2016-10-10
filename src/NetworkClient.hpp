#ifndef NetworkClient_hpp
#define NetworkClient_hpp 1

#include "global.hpp"

#include "Subject.hpp"
#include "IObserver.hpp"

class NetworkClient
	: public Subject
	, public IObserver
{
public:
	// const static Event s_Event_EV_NetworkClient_DISCONNECT = Event();

	enum EventEnum
	{
		EV_DISCONNECT, // Client has disconnected unexpectedly
		EV_QUIT // Client has disconnected normally (most likely due to the associated player quitting the game)
	};

	// The TCPsocket type is an opaque POINTER
	NetworkClient (const TCPsocket& socket, const std::string& name)
		: m_socket(socket)
		, m_suid(++s_suidCount)
		, m_name(name)
		, m_pThread(0)
	{
		Initialize();
	}

	NetworkClient (const NetworkClient& other)
		: m_socket(other.m_socket)
		, m_suid(++s_suidCount)
		, m_name(other.m_name)
		, m_pThread(other.m_pThread)
	{
		Initialize();
	}

	~NetworkClient ();

	NetworkClient& operator= (const NetworkClient& other)
	{
		m_socket = other.m_socket;
		m_suid = ++s_suidCount;
		m_name = other.m_name;
		m_pThread = other.m_pThread;

		return *this;
	}

	void Notify (const Subject*, int, const void*, const void*);

	typedef int (*ThreadFunction) (void*);
	// Spawn a new thread for the client and continue processing
	// A non-member or static member function of signature
	// int (*) (void*) is expected as input. If not provided,
	// then the default implementation is executed.
	bool Spawn (NetworkClient::ThreadFunction fn=NetworkClient::DefaultThreadFunction);

	bool operator== (const NetworkClient& other) const
	{
		return m_suid == other.m_suid; // TODO: Is this sufficient?
	}

private:
	void Initialize ();

	// Default function to be executed when a new client thread is spawned
	static int DefaultThreadFunction (void*);

	static size_t s_suidCount;

	// Original socket connection
	TCPsocket m_socket;

	size_t m_suid;
	std::string m_name; // TODO: should contain a Player object with player ID, name, etc.

	SDL_Thread* m_pThread;

public:
	inline const TCPsocket& GetSocket () const
	{
		return m_socket;
	}

	inline size_t GetID () const
	{
		return m_suid;
	}

	inline const std::string& GetName () const
	{
		return m_name;
	}

	// String rep
	inline std::string str () const
	{
		return m_name;
	}
};

#endif
