#include "ServerSocket.h"

#include "Wsa.h"

ServerSocket::ServerSocket(AddressFamily addressFamily, Protocol protocol) : Socket(addressFamily, Type::Stream, protocol)
{
}

void ServerSocket::bind(const SocketAddress& socketAddress)
{
	if (!isValid())
	{
		throw std::runtime_error("The socket wasn't opened");
	}

	if (isBound())
	{
		throw std::runtime_error("The socket already was bound.");
	}

	auto socketAddressIn = socketAddress.generateSocketAddressIn();
	::bind(socketDescriptor, reinterpret_cast<sockaddr*>(&socketAddressIn), sizeof(socketAddressIn));
	Wsa::instance().requireNoErrors();

	boundAddress = socketAddressIn;
}

bool ServerSocket::isBound() const
{
	return boundAddress.has_value();
}

void ServerSocket::close()
{
	Socket::close();

	boundAddress.reset();
}

void TCPServerSocket::listen(int maxConnectionsCount)
{
	if (type == Type::Dgram)
	{
		throw std::runtime_error("You can't use function 'listen' with Dgram protocol.");
	}

	::listen(socketDescriptor, maxConnectionsCount);
	Wsa::instance().requireNoErrors();
}

TCPClientSocket TCPServerSocket::accept() const
{
	if (type == Type::Dgram)
	{
		throw std::runtime_error("You can't use function 'accept' with Dgram protocol.");
	}

	sockaddr_in connectedAddress{};
	int new_len = sizeof(connectedAddress);
	ZeroMemory(&connectedAddress, sizeof(connectedAddress));
	SOCKET connectedSocket = ::accept(socketDescriptor, reinterpret_cast<sockaddr*>(&connectedAddress), &new_len);
	Wsa::instance().requireNoErrors();

	TCPClientSocket clientSocket;
	TCPClientSocketBridge tcpClientSocketBridge(clientSocket);
	tcpClientSocketBridge.fillUp(connectedSocket, connectedAddress);

	return clientSocket;
}

bool TCPServerSocket::isCanAccept() const
{
	fd_set fd;
	u_long nbio = 1;
	::ioctlsocket(socketDescriptor, FIONBIO, &nbio);
	FD_ZERO(&fd);
	FD_SET(socketDescriptor, &fd);
	timeval tv{60, 1};
	const auto result = select(0, &fd, nullptr, nullptr, &tv) > 0;
	nbio = 0;
	::ioctlsocket(socketDescriptor, FIONBIO, &nbio);
	return result;
}

void TCPServerSocket::open(AddressFamily addressFamily, Socket::Protocol protocol)
{
	Socket::open(addressFamily, Type::Stream, protocol);
}

void UDPServerSocket::open(AddressFamily addressFamily, Socket::Protocol protocol)
{
	Socket::open(addressFamily, Type::Dgram, protocol);
}

UDPServerSocket::Result UDPServerSocket::receive(std::size_t size)
{
	Result result;
	result.data.resize(size);

	sockaddr_in cliAddr{};
	int cliAddrLen = sizeof(cliAddr);

	recvfrom(socketDescriptor, result.data.data(), size, 0, (struct sockaddr*) &cliAddr, &cliAddrLen);
	Wsa::instance().requireNoErrors();

	result.client.fromSockaddrIn(cliAddr);

	return result;
}

void UDPServerSocket::sendTo(const std::string& data, const SocketAddress& address)
{
	std::vector<char> temp(data.begin(), data.end());
	sendTo(temp, address);
}

void UDPServerSocket::sendTo(const std::vector<char>& data, const SocketAddress& address)
{
	auto socketAddrIn = address.generateSocketAddressIn();
	::sendto(socketDescriptor, data.data(), data.size(), 0, reinterpret_cast<sockaddr*>(&socketAddrIn), sizeof(socketAddrIn));
	Wsa::instance().requireNoErrors();
}
