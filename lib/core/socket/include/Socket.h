#pragma once

#include "CopyableAndMoveable.h"
#include "NotCopyableButMovable.h"
#include "winsock2.h"

#include <string>

enum class AddressFamily
{
	None,
	Inet = AF_INET,
	Inet6 = AF_INET6,
};

class SocketAddress : public Utils::CopyableAndMoveable
{
public:
	SocketAddress() = default;
	SocketAddress(const std::string& address, USHORT port, AddressFamily addressFamily = AddressFamily::Inet);

	void setPort(USHORT port);
	[[nodiscard]] USHORT getPort() const;

	void setAddress(const std::string& address, AddressFamily addressFamily);
	[[nodiscard]] const std::string& getAddress() const;
	[[nodiscard]] IN_ADDR getAddressRaw() const;
	[[nodiscard]] sockaddr_in generateSocketAddressIn() const;
	void fromSockaddrIn(const sockaddr_in& sockaddr);

private:
	USHORT port = 0;
	IN_ADDR address{};
	std::string addressString;
	AddressFamily addressFamily = AddressFamily::Inet;
};

class Socket : public Utils::NotCopyableButMovable
{
public:
	enum class Type
	{
		None,
		Stream = SOCK_STREAM,
		Dgram = SOCK_DGRAM,
	};

	enum class Protocol
	{
		Auto = 0,
		Tcp = IPPROTO_TCP,
		Udp = IPPROTO_UDP,
	};

	inline static SOCKET invalidSocket = INVALID_SOCKET;

	Socket() = default;
	Socket(AddressFamily addressFamily, Type type, Protocol protocol = Protocol::Auto);
	~Socket() override;
	Socket(Socket&& other) noexcept;
	Socket& operator=(Socket&& other) noexcept;

	[[nodiscard]] bool isValid() const;
	[[nodiscard]] SOCKET getSocket() const;

	void open(AddressFamily addressFamily, Type type, Protocol protocol = Protocol::Auto);
	virtual void close();

	[[nodiscard]] Type getType() const;

	[[nodiscard]] SocketAddress getOwnAddress() const;

protected:
	SOCKET socketDescriptor = Socket::invalidSocket;
	AddressFamily addressFamily = AddressFamily::Inet;
	Type type = Type::None;
};