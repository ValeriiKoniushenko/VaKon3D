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

#include "Texture2D.h"

Texture2D::Texture2D()
{
	texture_.generate();
	texture_.bind();
}

int Texture2D::getWidth() const
{
	return image_.getWidth();
}

int Texture2D::getHeight() const
{
	return image_.getHeight();
}

glm::ivec2 Texture2D::getSize() const
{
	return image_.getSize();
}

Image::Channel Texture2D::getChannel() const
{
	return image_.getChannel();
}

unsigned char* Texture2D::imageData()
{
	return image_.data();
}

const unsigned char* Texture2D::imageData() const
{
	return image_.data();
}

void Texture2D::setInternalChannel(Gl::Texture::Channel channel)
{
	image_.setInternalChannel(channel);
}

Gl::Texture::Channel Texture2D::getInternalChannel() const
{
	return image_.getInternalChannel();
}

void Texture2D::loadImage(const std::filesystem::path& path, bool isFlipVertically /* = true*/)
{
	image_.loadImage(path, isFlipVertically);
	texture_.setImage(image_);
}

void Texture2D::loadImage(std::filesystem::path&& path, bool isFlipVertically /* = true*/)
{
	image_.loadImage(path, isFlipVertically);
	texture_.setImage(image_);
}

void Texture2D::loadToGpu()
{
	image_.loadToGpu();
}

void Texture2D::clear()
{
	image_.clear();
}

bool Texture2D::isEmpty() const
{
	return image_.isEmpty();
}

std::string Texture2D::getName() const
{
	return image_.getName();
}

nlohmann::json Texture2D::toJson() const
{
	nlohmann::json json;

	json["width"] = getWidth();
	json["height"] = getHeight();
	json["channel"] = Image::channelToString(getChannel());
	json["internalChannel"] = Gl::Texture::channelToString(getInternalChannel());
	json["isEmpty"] = isEmpty();
	json["name"] = getName();
	json["magFilter"] = Gl::Texture::magFilterToString(getMagFilter());
	json["minFilter"] = Gl::Texture::minFilterToString(getMinFilter());

	return json;
}

void Texture2D::setMagFilter(Gl::Texture::MagFilter filter)
{
	texture_.setMagFilter(filter);
}

Gl::Texture::MagFilter Texture2D::getMagFilter() const
{
	return texture_.getMagFilter();
}

void Texture2D::setMinFilter(Gl::Texture::MinFilter filter)
{
	texture_.setMinFilter(filter);
}

Gl::Texture::MinFilter Texture2D::getMinFilter() const
{
	return texture_.getMinFilter();
}

GLuint Texture2D::glData()
{
	return texture_.data();
}

Texture& Texture2D::getRawTexture()
{
	return texture_;
}
