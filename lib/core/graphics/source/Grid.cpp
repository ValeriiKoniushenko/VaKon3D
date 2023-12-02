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

#include "Grid.h"

#include "Camera.h"
#include "ShaderPack.h"

Grid::Grid()
{
	vao.generate();
	vbo.generate();

	vao.bind();
	vbo.bind();

	Gl::Vao::vertexAttribPointer(0, 3, Gl::Type::Float, false, 3 * sizeof(float), nullptr);
}

void Grid::generate()
{
	std::vector<float> vertices;

	size_ = static_cast<float>(count_) * gap_;

	for (int i = -count_; i <= count_; ++i)
	{
		glm::vec3 start = glm::vec3(static_cast<float>(gap_) * static_cast<float>(i), 0.f, -size_);
		glm::vec3 end = glm::vec3(static_cast<float>(gap_) * static_cast<float>(i), 0.f, size_);
		vertices.push_back(start.x);
		vertices.push_back(start.y);
		vertices.push_back(start.z);
		vertices.push_back(end.x);
		vertices.push_back(end.y);
		vertices.push_back(end.z);
	}

	for (int i = -count_; i <= count_; ++i)
	{
		glm::vec3 start = glm::vec3(size_, 0.f, static_cast<float>(gap_) * static_cast<float>(i));
		glm::vec3 end = glm::vec3(-size_, 0.f, static_cast<float>(gap_) * static_cast<float>(i));
		vertices.push_back(start.x);
		vertices.push_back(start.y);
		vertices.push_back(start.z);
		vertices.push_back(end.x);
		vertices.push_back(end.y);
		vertices.push_back(end.z);
	}

	vbo.bind();
	vbo.data(vertices);
}

void Grid::setColor(const Color4& color)
{
	lineColor_ = color;
}

void Grid::draw(ShaderPack& shaderPack, const Lightning& lightning, Camera& camera)
{
	auto& shader = shaderPack["line"];
	shader.use();

	shader.uniform("uViewPosition", camera.getPosition());
	shader.uniform("uLineColor", toGlColor4(lineColor_));
	shader.uniform("uProjectionView", false, camera.getMatrix());
	shader.uniform("uFogColor", toGlColor4(lightning.fog.color));
	shader.uniform("uFogMinDistance", lightning.fog.minDistance);
	shader.uniform("uFogMaxDistance", lightning.fog.maxDistance);

	vao.bind();
	glDepthFunc(GL_ALWAYS);
	glLineWidth(width_);
	Gl::drawArrays(GL_LINES, 0, count_ * 2 * 2 * 2);
	glDepthFunc(GL_LESS);
}

void Grid::setWidth(GLfloat width)
{
	width_ = width;
}

GLfloat Grid::getWidth() const
{
	return width_;
}

void Grid::setSize(int count)
{
	count_ = count;
	generate();
}
