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

#include "Commands.h"

#include "SceneObject.h"
#include "SceneObjectCollector.h"
#include "UtilsFunctions.h"

#include <iostream>

void CommandOutlineForAll::run(const std::vector<std::string>& args)
{
	if (args.empty())
	{
		response_ = "No args use next template: " + getHints();
		return;
	}

	int status = atoi(args.front().c_str());

	std::size_t count = 0;
	for (auto object : GetSceneObjectCollector())
	{
		++count;
		object->setOutlineStatus(status);
	}
	response_ += "Successful command executing. Object outlining was ";
	response_ += (status ? "enabled" : "disabled");
	response_ += " for " + std::to_string(count) + "pcs of Objects";
}

void CommandPrint::run(const std::vector<std::string>& args)
{
	for (auto& el : args)
	{
		std::cout << el << " ";
	}
	std::cout << std::endl;
	response_ = "Successful command executing";
}

void CommandGetObject::run(const std::vector<std::string>& args)
{
	response_ = "Objects: \n";
	for (const auto& arg : args)
	{
		try
		{
			if (Utils::isNumber(arg))
			{
				if (SceneObject* obj = GetSceneObjectCollector().at(atoi(arg.c_str())))
				{
					response_ += obj->toJson().dump(3) + "\n";
				}
				else
				{
					response_ += "No such object: " + arg + "\n";
				}
			}
			else
			{
				if (SceneObject* obj = GetSceneObjectCollector().at(arg))
				{
					response_ += obj->toJson().dump(3) + "\n";
				}
				else
				{
					response_ += "No such object: " + arg + "\n";
				}
			}
		}
		catch (std::runtime_error& error)
		{
			response_ += "No such object by index: " + arg + "\n";
		}
	}
}
