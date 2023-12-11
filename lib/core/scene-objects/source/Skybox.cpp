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

#include "Skybox.h"

#include "Camera.h"
#include "ShaderPack.h"
#include "glm/gtx/transform.hpp"

Skybox::Skybox() : texture_(Gl::Texture::Target::CubeMap)
{
}

void Skybox::loadFromFiles(const std::vector<std::filesystem::path>& pathToImages)
{
	if (pathToImages.size() != imagesCount)
	{
		throw std::runtime_error("Count of path to images have to be equal 6");
	}

	texture_.generate();
	texture_.bind();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);

	for (std::size_t i = 0; i < pathToImages.size(); ++i)
	{
		images_[i].loadImage(pathToImages[i], false);
		images_[i].loadToGpu(static_cast<Gl::Texture::Target>(static_cast<std::size_t>(Gl::Texture::Target::CubeMapPositiveX) + i));
	}
	Gl::Texture::setWrapR(Gl::Texture::Wrap::Clamp2Edge, Gl::Texture::Target::CubeMap);
	Gl::Texture::setWrapS(Gl::Texture::Wrap::Clamp2Edge, Gl::Texture::Target::CubeMap);
	Gl::Texture::setWrapT(Gl::Texture::Wrap::Clamp2Edge, Gl::Texture::Target::CubeMap);
	Gl::Texture::setMinFilter(Gl::Texture::MinFilter::LinearMipmapLinear);
	Gl::Texture::setMagFilter(Gl::Texture::MagFilter::Linear);

	Gl::Texture::generateMipmap(Gl::Texture::Target::CubeMap);
}

void Skybox::draw(ShaderPack& shaderPack, Camera& camera)
{
	auto& shader = shaderPack["skybox"];
	shader.use();

	if (!vao_.isGenerated())
	{
		vao_.generate();
	}

	vao_.bind();

	if (!vbo_.isGenerated())
	{
		vbo_.generate();
	}

	vbo_.bind();

	const float size = 1000.f;
	// clang-format off
	vbo_.data({
		-size,  size, -size,
		-size, -size, -size,
		size, -size, -size,
		size, -size, -size,
		size,  size, -size,
		-size,  size, -size,

		-size, -size,  size,
		-size, -size, -size,
		-size,  size, -size,
		-size,  size, -size,
		-size,  size,  size,
		-size, -size,  size,

		size, -size, -size,
		size, -size,  size,
		size,  size,  size,
		size,  size,  size,
		size,  size, -size,
		size, -size, -size,

		-size, -size,  size,
		-size,  size,  size,
		size,  size,  size,
		size,  size,  size,
		size, -size,  size,
		-size, -size,  size,

		-size,  size, -size,
		size,  size, -size,
		size,  size,  size,
		size,  size,  size,
		-size,  size,  size,
		-size,  size, -size,

		-size, -size, -size,
		-size, -size,  size,
		size, -size, -size,
		size, -size, -size,
		-size, -size,  size,
		size, -size,  size
	});
	// clang-format on

	Gl::Texture::active(0);
	texture_.bind();

	Gl::Vao::vertexAttribPointer(0, 3, Gl::Type::Float, false, 3 * sizeof(float), nullptr);

	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, camera.getPosition());

	shader.uniform("uProjectionView", false, camera.getMatrix());
	shader.uniform("uModel", false, model);
	shader.uniform("uSkybox", 0);

	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}