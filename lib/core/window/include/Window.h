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

#include "Color.h"
#include "Delegate.h"
#include "GlfwWrapper.h"
#include "Singleton.h"
#include "Size.h"

#include <Windows.h>

#include <filesystem>

class Window : public Singleton<Window>
{
public:
	void create(Utils::ISize2D size, const std::string& title);
	[[nodiscard]] bool shouldClose() const;
	void swapBuffers();
	void pollEvent();
	void clearColor(const GlColor4& color);
	void clear(int code);
	void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
	[[nodiscard]] HWND getHwnd();
	[[nodiscard]] Utils::ISize2D getSize() const;
	void setCursorPosition(double xpos, double ypos);
	void setIcon(const std::filesystem::path& pathToIco);

	LambdaMulticastDelegate<void(int, int, int, int)> onKeyPressed;
	LambdaMulticastDelegate<void(unsigned int)> onTextInput;
	LambdaMulticastDelegate<void(int)> onCursorEntered;
	LambdaMulticastDelegate<void(double, double)> onMouseWheel;

protected:
	GLFWwindow* window{};
	Utils::ISize2D size_{};
	std::string title_;
};

Window& GetWindow();