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
#include "NotCopyableButMovable.h"
#include "Vao.h"
#include "Vbo.h"
#include "glm/glm.hpp"

#include <vector>

class ShaderPack;
class Camera;

class Line : public Utils::NotCopyableButMovable
{
public:
	Line();

	void setStartAndEndPoint(glm::vec3 start, glm::vec3 end);

	void setColor(const Color4& color);

	void draw(ShaderPack& shaderPack, Camera& camera);

	void setWidth(GLfloat width);
	[[nodiscard]] GLfloat getWidth() const;

	[[nodiscard]] std::pair<glm::vec3, glm::vec3> getLine() const;

	[[nodiscard]] bool isNull() const;

private:
	GLfloat width_{1.f};
	Vbo vbo;
	Vao vao;
	glm::vec3 startPoint_{};
	glm::vec3 endPoint_{};
	Color4 lineColor_;
};