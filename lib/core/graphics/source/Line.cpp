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

#include "Line.h"

#include "Camera.h"
#include "ShaderPack.h"

Line::Line()
{
	vao.generate();
	vbo.generate();

	vao.bind();
	vbo.bind();

	Gl::Vao::vertexAttribPointer(0, 3, Gl::Type::Float, false, 3 * sizeof(float), nullptr);
}

void Line::setStartAndEndPoint(glm::vec3 start, glm::vec3 end)
{
	startPoint_ = start;
	endPoint_ = end;

	std::vector<float> vertices = {
		start.x,
		start.y,
		start.z,
		end.x,
		end.y,
		end.z,
	};

	vbo.bind();
	vbo.data(vertices);
}

void Line::setColor(const Color4& color)
{
	lineColor_ = color;
}

int Line::draw(ShaderPack& shaderPack, Camera& camera)
{
	auto& shader = shaderPack["line"];
	shader.use();

	shader.uniform("uLineColor", toGlColor4(lineColor_));
	shader.uniform("uProjectionView", false, camera.getMatrix());

	vao.bind();
	glLineWidth(width_);
	Gl::drawArrays(GL_LINES, 0, 2);
	return 1;
}

void Line::setWidth(GLfloat width)
{
	width_ = width;
}

GLfloat Line::getWidth() const
{
	return width_;
}
