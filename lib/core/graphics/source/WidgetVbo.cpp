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

#include "WidgetVbo.h"

WidgetVbo::WidgetVbo(bool shouldGenerate, bool shouldBind) : Vbo(shouldGenerate, shouldBind)
{
}

void WidgetVbo::data(const std::vector<Unit>& vertices, GLenum usage) const
{
	if (!isGenerated())
	{
		throw std::runtime_error("VBO is empty. Try to regenerate and try again.");
	}

	if (!isBind())
	{
		throw std::runtime_error("You try to put data to unbound VBO. Bind it and try again");
	}

	Gl::Vbo::data(GL_ARRAY_BUFFER, sizeof(Unit) * vertices.size(), vertices.data(), usage);
}
