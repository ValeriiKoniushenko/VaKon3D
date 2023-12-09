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
class Lightning;
class Camera;

class Grid : public Utils::NotCopyableButMovable
{
public:
	Grid();

	void setColor(const Color4& color);

	void draw(ShaderPack& shaderPack, const Lightning& lightning, Camera& camera);

	void setLineWidth(GLfloat width);
	[[nodiscard]] GLfloat getWidth() const;

	void setSize(int count);

private:
	void generate();

private:
	int count_ = 10'000;
	float gap_ = 100.f;
	float size_ = 0.f;
	GLfloat width_{2.f};
	Vbo vbo;
	Vao vao;
	Color4 lineColor_ = {92, 107, 96, 49};
	bool isDirty_ = true;
};