#ifndef NetworkClient_hpp
#define NetworkClient_hpp 1

#include "global.hpp"

class NetworkClient
{
public:
	// TODO: Spawn a new thread!
	NetworkClient (const TCPsocket& socket, const std::string& name)
		: m_socket(socket)
		, m_suid(++s_suidCount)
		, m_name(name)
	{}

	NetworkClient (const NetworkClient& other)
		: m_socket(other.m_socket)
		, m_suid(++s_suidCount)
		, m_name(other.m_name)
	{}

	NetworkClient& operator= (const NetworkClient& other)
	{
		m_socket = other.m_socket;
		m_suid = ++s_suidCount;
		m_name = other.m_name;

		return *this;
	}

private:
	static size_t s_suidCount;

	// Original socket connection
	TCPsocket m_socket;

	size_t m_suid;
	std::string m_name; // TODO: should contain a Player object with player ID, name, etc.

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
};

#endif
