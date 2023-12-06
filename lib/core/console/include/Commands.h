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

#include "CopyableAndMoveable.h"

#include <string>
#include <vector>

class BaseCommand : public Utils::CopyableAndMoveable
{
public:
	explicit BaseCommand(const std::string& commandName, const std::string& hints) : commandName_(commandName), hints_(hints)
	{
	}

	virtual void run(const std::vector<std::string>& args) = 0;

	[[nodiscard]] virtual std::string getHints() const
	{
		return hints_;
	};

	[[nodiscard]] virtual std::string getResponse() const
	{
		return response_;
	}

	[[nodiscard]] std::string getCommandName() const
	{
		return commandName_;
	}

protected:
	const std::string hints_;
	const std::string commandName_;
	std::string response_;
};

class CommandOutlineForAll : public BaseCommand
{
public:
	CommandOutlineForAll() : BaseCommand("outline-for-all", "outline-for-all <1|0>")
	{
	}

	void run(const std::vector<std::string>& args) override;
};

class CommandPrint : public BaseCommand
{
public:
	CommandPrint() : BaseCommand("print", "print <message1> <message2> ...")
	{
	}

	void run(const std::vector<std::string>& args) override;
};

class CommandGetObject : public BaseCommand
{
public:
	CommandGetObject() : BaseCommand("get-object", "get-object <index|name>")
	{
	}

	void run(const std::vector<std::string>& args) override;
};
