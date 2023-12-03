#pragma once

#include "ClientSocket.h"
#include "Socket.h"

#include <optional>

class ServerSocket : public Socket
{
public:
	ServerSocket() = default;
	ServerSocket(AddressFamily addressFamily, Protocol protocol = Protocol::Auto);

	void bind(const SocketAddress& socketAddress);
	[[nodiscard]] bool isBound() const;
	void close() override;

private:
	std::optional<sockaddr_in> boundAddress{};
};

class TCPServerSocket : public ServerSocket
{
public:
	void listen(int maxConnectionsCount = SOMAXCONN);
	[[nodiscard]] TCPClientSocket accept() const;
	[[nodiscard]] bool isCanAccept() const;
	void open(AddressFamily addressFamily, Protocol protocol = Protocol::Auto);
};

class UDPServerSocket : public ServerSocket
{
public:
	struct Result
	{
		SocketAddress client;
		std::vector<char> data;
	};

	void open(AddressFamily addressFamily, Protocol protocol = Protocol::Auto);
	Result receive(std::size_t size);
	void sendTo(const std::string& data, const SocketAddress& address);
	void sendTo(const std::vector<char>& data, const SocketAddress& address);
};