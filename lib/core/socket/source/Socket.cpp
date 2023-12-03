#include "Socket.h"

#include "Wsa.h"

#include <ws2tcpip.h>

Socket::~Socket()
{
	close();
}

bool Socket::isValid() const
{
	return socketDescriptor != Socket::invalidSocket;
}

SOCKET Socket::getSocket() const
{
	return socketDescriptor;
}

void Socket::close()
{
	if (isValid())
	{
		closesocket(socketDescriptor);
		socketDescriptor = Socket::invalidSocket;
	}
}

Socket::Socket(AddressFamily addressFamily, Socket::Type type, Socket::Protocol protocol /* = Protocol::Auto*/)
{
	open(addressFamily, type, protocol);
}

void Socket::open(AddressFamily addressFamily, Socket::Type type, Socket::Protocol protocol /* = Protocol::Auto*/)
{
	close();

	socketDescriptor = socket(static_cast<int>(addressFamily), static_cast<int>(type), static_cast<int>(protocol));
	this->type = type;

	if (socketDescriptor == Socket::invalidSocket)
	{
		Wsa::instance().requireNoErrors();
	}
}

Socket& Socket::operator=(Socket&& other) noexcept
{
	close();
	socketDescriptor = other.socketDescriptor;
	addressFamily = other.addressFamily;
	type = other.type;

	other.socketDescriptor = Socket::invalidSocket;

	return *this;
}

Socket::Socket(Socket&& other) noexcept
{
	*this = std::move(other);
}

Socket::Type Socket::getType() const
{
	return type;
}

SocketAddress Socket::getOwnAddress() const
{
	sockaddr_in addr;
	int addLen = sizeof(addr);
	::getsockname(socketDescriptor, reinterpret_cast<sockaddr*>(&addr), &addLen);
	Wsa::instance().requireNoErrors();

	SocketAddress address;
	address.fromSockaddrIn(addr);
	return address;
}

void SocketAddress::setPort(USHORT port)
{
	this->port = htons(port);
}

USHORT SocketAddress::getPort() const
{
	return ntohs(port);
}

void SocketAddress::setAddress(const std::string& address, AddressFamily addressFamily)
{
	if (addressFamily != AddressFamily::Inet && addressFamily != AddressFamily::Inet6)
	{
		throw std::runtime_error("Invalid Address Family for setting the socket's address. You have to use IPv4 or IPv6");
	}

	addressString = address;
	inet_pton(static_cast<int>(addressFamily), address.c_str(), &this->address);
}

const std::string& SocketAddress::getAddress() const
{
	return addressString;
}

IN_ADDR SocketAddress::getAddressRaw() const
{
	return address;
}

sockaddr_in SocketAddress::generateSocketAddressIn() const
{
	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr = address;

	return addr;
}

SocketAddress::SocketAddress(const std::string& address, USHORT port, AddressFamily addressFamily)
{
	setPort(port);
	setAddress(address, addressFamily);
}

void SocketAddress::fromSockaddrIn(const sockaddr_in& sockaddr)
{
	port = sockaddr.sin_port;
	address = sockaddr.sin_addr;

	char str[INET_ADDRSTRLEN]{};
	inet_ntop(sockaddr.sin_family, &(sockaddr.sin_addr), str, INET_ADDRSTRLEN);
	addressString = str;
}