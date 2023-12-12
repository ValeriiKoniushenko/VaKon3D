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

#include "Widget.h"

#include "ShaderPack.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Window.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

void Widget::setTexture(Texture& texture)
{
	texture_ = &texture;
	isDirtyTexture_ = true;
}

void Widget::setTexture(Texture2D& texture)
{
	texture_ = &texture.getRawTexture();
	isDirtyTexture_ = true;
}

Texture* Widget::getTexture()
{
	return texture_;
}

const Texture* Widget::getTexture() const
{
	return texture_;
}

void Widget::resetTexture()
{
	texture_ = nullptr;
}

void Widget::draw(ShaderPack& shaderPack)
{
	auto& shader = shaderPack["widget"];
	shader.use();

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

	if (texture_)
	{
		Gl::Texture::active(0);
		texture_->bind();
		if (isDirtyTexture_)
		{
			texture_->loadToGpu();
			isDirtyTexture_ = false;
		}
	}

	if (isDirtyVertices_)
	{
		auto vertices = verticesTemplate_;
		vertices[1].position.x *= size_.width;
		vertices[2].position.x *= size_.width;
		vertices[2].position.y *= size_.height;
		vertices[4].position.x *= size_.width;
		vertices[4].position.y *= size_.height;
		vertices[5].position.y *= size_.height;

		vbo_.bind();
		vbo_.data(vertices);
		isDirtyVertices_ = false;
	}

	Gl::Vao::vertexAttribPointer(1, 2, Gl::Type::Float, false, 4 * sizeof(float), nullptr);
	Gl::Vao::vertexAttribPointer(2, 2, Gl::Type::Float, false, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(GetWindow().getSize().width), 0.0f,
		static_cast<float>(GetWindow().getSize().height), 0.1f, 1000.0f);

	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(position_, 0.f));
	model = glm::rotate(model, glm::radians(rotate_), glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, glm::vec3(origin_, 0.f));

	shader.uniform("uProjection", false, proj);
	shader.uniform("uModel", false, model);

	Gl::drawArrays(GL_TRIANGLES, 0, verticesTemplate_.size());

	if (texture_)
	{
		texture_->unbind();
	}
}

void Widget::setSize(Utils::FSize2D size)
{
	size_ = size;
	isDirtyVertices_ = true;
}

Utils::FSize2D Widget::getSize() const
{
	return size_;
}

Widget::Widget(Texture& texture)
{
	setTexture(texture);
}

void Widget::setPosition(glm::vec2 position)
{
	position_ = position;
}

glm::vec2 Widget::getPosition() const
{
	return position_;
}

void Widget::move(glm::vec2 offset)
{
	position_ += offset;
}

void Widget::setRotate(float degrees)
{
	rotate_ = degrees;
}

float Widget::getRotate() const
{
	return rotate_;
}

void Widget::rotate(float degrees)
{
	rotate_ += degrees;
	while (rotate_ >= 360.f)
	{
		rotate_ -= 360.f;
	}
}

void Widget::setOrigin(glm::vec2 origin)
{
	origin_ = origin;
}

glm::vec2 Widget::getOrigin() const
{
	return origin_;
}

nlohmann::json Widget::toJson() const
{
	nlohmann::json json;
	return json;
}