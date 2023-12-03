#pragma once

#include "Socket.h"

#include <functional>
#include <optional>
#include <vector>

class TCPClientSocket : public Socket
{
public:
	TCPClientSocket();
	TCPClientSocket(AddressFamily addressFamily, Protocol protocol = Protocol::Auto);
	TCPClientSocket(TCPClientSocket&& other) noexcept;
	TCPClientSocket& operator=(TCPClientSocket&& other) noexcept;

	void close() override;
	[[nodiscard]] bool isConnected() const;
	[[nodiscard]] SocketAddress getAddress() const;
	bool connect();
	bool connectTo(const SocketAddress& socketAddress);
	bool connectByHostName(const std::string& address, short port = 80);

	void send(const std::string& data);
	void send(const std::vector<char>& data);

	void open(AddressFamily addressFamily, Protocol protocol = Protocol::Auto);

	std::string receiveAsString(std::size_t receiveSize);
	std::vector<char> receive(std::size_t receiveSize);
	void receiveTo(std::size_t receiveSize, std::function<void(const char*, std::size_t)>&& callback);

private:
	sockaddr_in connectedAddress;
	bool isConnected_ = false;
	friend class TCPClientSocketBridge;
};

class TCPClientSocketBridge
{
public:
	explicit TCPClientSocketBridge(TCPClientSocket& clientSocket);
	void fillUp(SOCKET socket, const sockaddr_in& address);

private:
	TCPClientSocket& clientSocket;
};

class UDPClientSocket : public Socket
{
public:
	struct Result
	{
		SocketAddress server;
		std::vector<char> data;
	};

	UDPClientSocket() = default;
	explicit UDPClientSocket(AddressFamily addressFamily, Protocol protocol = Protocol::Auto);
	void sendTo(const std::string& message, const SocketAddress& socketAddress);
	Result receive(std::size_t size);

private:
	friend class UDPClientSocketBridge;
};
