// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "EditorIntegration.h"

#include "EditorNetworkProtocol.h"

#include <Windows.h>

#include <iostream>
#include <stdexcept>

EditorIntegration::EditorIntegration()
{
}

EditorIntegration::~EditorIntegration()
{
	mainNetworkThread.request_stop();
}

void EditorIntegration::connectToEditor()
{
	std::jthread tmp(
		[this](std::stop_token token)
		{
			while (!token.stop_requested())
			{
				while (!clientSocket.isConnected() && !token.stop_requested())
				{
					try
					{
						clientSocket.open(AddressFamily::Inet);
						clientSocket.connectTo(SocketAddress("127.0.0.1", 48989));
					}
					catch (...)
					{
						// do nothing
					}
				}
				if (!clientSocket.isConnected())
				{
					return;
				}

				try
				{
					while (!token.stop_requested())
					{
						auto request = getEditorRequest();
						auto actions = request[EditorNetworkProtocol::Body::possibleActionsPropertyName];
						if (actions == "console")
						{
							const auto response = console.runCommand(request[EditorNetworkProtocol::Body::contentPropertyName]);
							giveResponseToTheConsole(response);
						}
					}
				}
				catch (...)
				{
					MessageBoxA(nullptr, "Was got an exception", "Editor-Connection error", MB_OK);
				}
			}
		});

	mainNetworkThread = std::move(tmp);
}

nlohmann::json EditorIntegration::getEditorRequest()
{
	auto json = nlohmann::json::parse(clientSocket.receiveAsString(EditorNetworkProtocol::Header::length));

	const std::size_t bodyLength = json[EditorNetworkProtocol::Header::lengthPropertyName];
	auto tmp = clientSocket.receiveAsString(bodyLength + 1ull);
	return nlohmann::json::parse(tmp);
}

void EditorIntegration::giveResponseToTheConsole(const std::string& response)
{
	EditorNetworkProtocol::Body body;
	body.action = "response-to-console";
	body.content = response;
	auto bodyString = EditorNetworkProtocol::Body::generate(body);
	auto headerString = EditorNetworkProtocol::Header::generate(bodyString.length());

	std::vector<char> headerData(headerString.begin(), headerString.end());
	headerData.resize(EditorNetworkProtocol::Header::length);

	clientSocket.send(headerData);
	clientSocket.send(bodyString);
}
