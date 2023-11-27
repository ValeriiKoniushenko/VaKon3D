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

#include "Triangle.h"

#include "ShaderPack.h"
#include "Texture.h"
#include "Window.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Triangle::Triangle(Texture& texture)
{
	texture_ = &texture;
}

void Triangle::draw(ShaderPack& shaderPack)
{
	auto& shader = shaderPack["triangle"];
	shader.use();

	if (texture_)
	{
		texture_->bind();
		if (isDirtyTexture_)
		{
			texture_->loadToGpu();
			isDirtyTexture_ = false;
		}
	}
	if (!vao_.isGenerated())
	{
		vao_.generate();
	}
	if (!vbo_.isGenerated())
	{
		vbo_.generate();
	}

	vao_.bind();
	vbo_.bind();

	if (isDirtyVertices_)
	{
		auto vertices = verticesTemplate_;
		vertices[1].position.x *= size_.width;
		vertices[2].position.x *= size_.width;
		vertices[2].position.y *= size_.height;

		vbo_.bind();
		vbo_.data(vertices);
		isDirtyVertices_ = false;
	}

	Gl::Vao::vertexAttribPointer(1, 3, Gl::Type::Float, false, 8 * sizeof(float), nullptr);
	Gl::Vao::vertexAttribPointer(2, 2, Gl::Type::Float, false, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(3, 3, Gl::Type::Float, false, 8 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));

	const glm::mat4 proj = glm::perspective(glm::radians(190.0f),
		static_cast<float>(GetWindow().getSize().width) / static_cast<float>(GetWindow().getSize().height), 0.1f, 10000.0f);

	glm::mat4 model = glm::mat4(1.f);
	auto pos = position_;
	pos.x = -pos.x;
	pos.y = -pos.y;

	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rotate_), glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, origin_);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::translate(view, glm::vec3(0, 0, 0.f));

	shader.uniform("uProjection", false, proj);
	shader.uniform("uModel", false, model);
	shader.uniform("uView", false, view);

	Gl::drawArrays(GL_TRIANGLES, 0, verticesTemplate_.size());
}

void Triangle::setTexture(Texture& texture)
{
	texture_ = &texture;
	isDirtyTexture_ = true;
}

Texture* Triangle::getTexture()
{
	return texture_;
}

const Texture* Triangle::getTexture() const
{
	return texture_;
}

void Triangle::resetTexture()
{
	texture_ = nullptr;
}

void Triangle::setSize(Utils::FSize2D size)
{
	size_ = size;
	isDirtyVertices_ = true;
}

Utils::FSize2D Triangle::getSize() const
{
	return size_;
}

void Triangle::setPosition(glm::vec3 position)
{
	position_ = position;
}

void Triangle::move(glm::vec3 offset)
{
	position_ += offset;
}

glm::vec3 Triangle::getPosition() const
{
	return position_;
}

void Triangle::setRotate(float degrees)
{
	rotate_ = degrees;
}

float Triangle::getRotate() const
{
	return rotate_;
}

void Triangle::rotate(float degrees)
{
	rotate_ += degrees;
}

void Triangle::setOrigin(glm::vec3 origin)
{
	origin_ += origin;
}

glm::vec3 Triangle::getOrigin() const
{
	return origin_;
}
