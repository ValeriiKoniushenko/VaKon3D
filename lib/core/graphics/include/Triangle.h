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

#include "Size.h"
#include "TriangleVbo.h"
#include "Vao.h"

class Camera;
class ShaderPack;
class Texture;

class Triangle
{
public:
	const inline static GLsizei verticesCount = 3;

	Triangle() = default;
	explicit Triangle(Texture& texture);

	void draw(ShaderPack& shaderPack, Camera& camera, const glm::mat4& model);

	void setTexture(Texture& texture);
	[[nodiscard]] Texture* getTexture();
	[[nodiscard]] const Texture* getTexture() const;
	void resetTexture();

	void setVertices(std::vector<TriangleVbo::Unit> vertices);

private:
	Texture* texture_ = nullptr;
	Vao vao_;
	TriangleVbo vbo_;
	std::vector<TriangleVbo::Unit> vertices_;
	bool isDirtyVertices_ = true;
	bool isDirtyTexture_ = true;
};