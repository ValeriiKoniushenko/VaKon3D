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

#include "ConsoleConnector.h"

#include "ClientSocket.h"
#include "EditorNetworkProtocol.h"

#include <fstream>

ConsoleConnector::ConsoleConnector(TCPClientSocket& acceptedClient) : acceptedClient_(acceptedClient)
{
}

void ConsoleConnector::serializeConsoleCommands()
{
	std::ofstream file(pathToHistory);
	if (file.is_open())
	{
		for (auto& command : commands_)
		{
			file << command << std::endl;
		}
		file.close();
	}
}

void ConsoleConnector::deserializeConsoleCommands()
{
	std::ifstream file(pathToHistory);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			if (!line.empty())
			{
				onDeserializeOneCommand.trigger(line);
				commands_.emplace_back(line.c_str());
			}
		}

		file.close();
	}
}

void ConsoleConnector::addCommandToHistory(const std::string& command)
{
	commands_.emplace_back(command);
}

std::string ConsoleConnector::previousCommand()
{
	if (currentCommandIndex + 1 < commands_.size())
	{
		++currentCommandIndex;
	}
	return getCommandByIndexRevers(currentCommandIndex);
}

std::string ConsoleConnector::nextCommand()
{
	if (currentCommandIndex - 1 >= 0)
	{
		--currentCommandIndex;
	}

	return getCommandByIndexRevers(currentCommandIndex);
}

std::string ConsoleConnector::getCommandByIndex(std::size_t index)
{
	if (currentCommandIndex >= 0 && currentCommandIndex < commands_.size())
	{
		return commands_.at(currentCommandIndex);
	}
	return {};
}

std::string ConsoleConnector::getCommandByIndexRevers(std::size_t index)
{
	if (currentCommandIndex >= 0 && currentCommandIndex < commands_.size())
	{
		return commands_.at(commands_.size() - currentCommandIndex - 1ull);
	}
	return {};
}

void ConsoleConnector::executeConsoleCommand(const std::string& command)
{
	if (command == "cls")
	{
		onClearScreen.trigger();
		return;
	}

	{
		EditorNetworkProtocol::Body body;
		body.content = command;
		body.action = "console";
		const auto bodyString = EditorNetworkProtocol::Body::generate(body);
		const auto headerString = EditorNetworkProtocol::Header::generate(bodyString.size());

		std::vector<char> headerData(headerString.begin(), headerString.end());
		headerData.resize(EditorNetworkProtocol::Header::length);

		acceptedClient_.send(headerData);
		acceptedClient_.send(bodyString);
	}

	onAddCommand.trigger(command);
	addCommandToHistory(command);

	{
		auto header = nlohmann::json::parse(acceptedClient_.receiveAsString(EditorNetworkProtocol::Header::length));

		const std::size_t bodyLength = header[EditorNetworkProtocol::Header::lengthPropertyName];
		auto body = nlohmann::json::parse(acceptedClient_.receiveAsString(bodyLength + 1ull));

		if (body[EditorNetworkProtocol::Body::possibleActionsPropertyName] == "response-to-console")
		{
			onResponse.trigger(*body[EditorNetworkProtocol::Body::contentPropertyName].get<std::string*>());
		}
	}
}
