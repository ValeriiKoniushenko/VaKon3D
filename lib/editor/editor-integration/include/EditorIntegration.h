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

#include "ClientSocket.h"
#include "Console.h"
#include "NotCopyableButMovable.h"
#include "boost/property_tree/json_parser.hpp"

#include <thread>

struct EditorNetworkProtocol
{
	struct Header
	{
		inline static const std::size_t length = 128;
		inline static const std::string lengthPropertyName = "length";

		inline static const std::vector<std::string> possibleTypes = {"json"};
		inline static const std::string typePropertyName = "type";

		static std::string generate(std::size_t bodyLength)
		{
			boost::property_tree::ptree json;
			json.put(Header::lengthPropertyName, bodyLength);
			json.put(Header::typePropertyName, Header::possibleTypes.front());

			std::ostringstream oss;
			boost::property_tree::write_json(oss, json);
			return oss.str();
		}
	};

	struct Body
	{
		inline static const std::vector<std::string> possibleActions = {"console"};
		inline static const std::string possibleActionsPropertyName = "action";

		inline static const std::size_t maxContentLength = 1'073'741'824;	 // bytes
		inline static const std::string contentPropertyName = "content";

		std::string content;
		std::string action;

		static std::string generate(const Body& body)
		{
			boost::property_tree::ptree json;
			json.put(Body::possibleActionsPropertyName, body.action);
			json.put(Body::contentPropertyName, body.content);

			std::ostringstream oss;
			boost::property_tree::write_json(oss, json);
			return oss.str();
		}
	};
};

class EditorIntegration : public Utils::NotCopyableButMovable
{
public:
	EditorIntegration();
	~EditorIntegration() override;

	void connectToEditor();

	Console console;

private:
	boost::property_tree::ptree getEditorRequest();

private:
	TCPClientSocket clientSocket;
	std::jthread mainNetworkThread;
};