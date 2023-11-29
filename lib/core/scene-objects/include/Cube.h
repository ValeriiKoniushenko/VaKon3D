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

#include "SceneObject.h"
#include "Triangle.h"
#include "TriangleVbo.h"

class Cube : public SceneObject
{
public:
	inline static const std::size_t sidesCount = 12;

	void draw(ShaderPack& shaderPack, Camera& camera) override;
	[[nodiscard]] boost::property_tree::ptree toJson() const override;

	void setTexture(Texture& texture);
	[[nodiscard]] Texture* getTexture();
	[[nodiscard]] const Texture* getTexture() const;

	void setSize(float size);
	[[nodiscard]] float getSize() const;

	void update() override;

private:
	void setVertices();

private:
	float size_ = 100.f;
	bool isDirtyTexture_ = false;
	bool verticesAreDirty_ = true;
	Texture* texture_ = nullptr;
	TriangleVbo vbo_;
	Vao vao_;
	std::vector<TriangleVbo::Unit> triangles_;
};