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

#pragma once

#include "Delegate.h"
#include "NotCopyableAndNotMovable.h"

#include <filesystem>
#include <string>
#include <vector>

class TCPClientSocket;

class ConsoleConnector : public Utils::NotCopyableAndNotMovable
{
public:
	inline static const std::filesystem::path pathToHistory = "history.txt";

	ConsoleConnector(TCPClientSocket& acceptedClient);

	void deserializeConsoleCommands();
	void serializeConsoleCommands();
	void addCommandToHistory(const std::string& command);
	std::string previousCommand();
	std::string nextCommand();
	std::string getCommandByIndex(std::size_t index);
	std::string getCommandByIndexRevers(std::size_t index);
	void executeConsoleCommand(const std::string& command);

	LambdaMulticastDelegate<void(const std::string&)> onDeserializeOneCommand;
	LambdaMulticastDelegate<void()> onClearScreen;
	LambdaMulticastDelegate<void(const std::string&)> onAddCommand;
	LambdaMulticastDelegate<void(const std::string&)> onResponse;

private:
	TCPClientSocket& acceptedClient_;
	std::vector<std::string> commands_;
	int currentCommandIndex = -1;
};