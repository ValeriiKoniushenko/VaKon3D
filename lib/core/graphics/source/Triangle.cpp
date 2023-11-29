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

#include "Camera.h"
#include "Image.h"
#include "ShaderPack.h"
#include "Texture.h"
#include "Window.h"

Triangle::Triangle(Texture& texture)
{
	texture_ = &texture;
}

void Triangle::draw(ShaderPack& shaderPack, const Lightning& lightning, Camera& camera)
{
	SceneObject::draw(shaderPack, lightning, camera);

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

	if (verticesAreDirty_)
	{
		vbo_.data(vertices_);
		verticesAreDirty_ = false;
	}

	Gl::Vao::vertexAttribPointer(1, 3, Gl::Type::Float, false, 10 * sizeof(float), nullptr);
	Gl::Vao::vertexAttribPointer(2, 2, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(3, 3, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(4, 2, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(8 * sizeof(float)));

	shader.uniform("uProjectionAndView", false, camera.getMatrix());
	shader.uniform("uModel", false, cachedModelMatrix_);

	shader.uniform("uAmbientLightColor", lightning.ambient.lightColor);
	shader.uniform("uAmbientLightDirection", lightning.ambient.direction);
	shader.uniform("uAmbientLightMaxDark", lightning.ambient.maxDark);

	shader.uniform("uSpecularColor", lightning.specular.lightColor);
	shader.uniform("uSpecularPosition", lightning.specular.position);
	shader.uniform("uSpecularIntensity", lightning.specular.intensity);
	shader.uniform("uSpecularPow", lightning.specular.specularPow);
	shader.uniform("uViewPosition", camera.getPosition());

	if (texture_)
	{
		shader.uniform("uAtlasSize", texture_->getImage()->getSize());
	}

	Gl::drawArrays(GL_TRIANGLES, 0, Triangle::verticesCount);

	if (texture_)
	{
		texture_->unbind();
	}
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

void Triangle::setVertices(std::vector<TriangleVbo::Unit> vertices)
{
	if (vertices.size() != Triangle::verticesCount)
	{
		throw std::runtime_error("Impossible to set vertices. The vertices count must be equal to three");
	}
	vertices_ = std::move(vertices);
	verticesAreDirty_ = true;
}

boost::property_tree::ptree Triangle::toJson() const
{
	return boost::property_tree::ptree();
}
