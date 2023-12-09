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

#include "Image.h"
#include "JsonPrintable.h"
#include "NotCopyableButMovable.h"
#include "Texture.h"

class Texture2D : public Utils::NotCopyableButMovable, public JsonPrintable
{
public:
	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;
	[[nodiscard]] glm::ivec2 getSize() const;
	[[nodiscard]] Image::Channel getChannel() const;
	[[nodiscard]] unsigned char* imageData();
	[[nodiscard]] const unsigned char* imageData() const;
	void setInternalChannel(Gl::Texture::Channel channel);
	[[nodiscard]] Gl::Texture::Channel getInternalChannel() const;
	void loadImage(const std::filesystem::path& path, bool isFlipVertically = true);
	void loadImage(std::filesystem::path&& path, bool isFlipVertically = true);
	void loadToGpu();
	void clear();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] nlohmann::json toJson() const override;
	void setMagFilter(Gl::Texture::MagFilter filter);
	[[nodiscard]] Gl::Texture::MagFilter getMagFilter() const;
	void setMinFilter(Gl::Texture::MinFilter filter);
	[[nodiscard]] Gl::Texture::MinFilter getMinFilter() const;
	[[nodiscard]] GLuint glData();

private:
	Texture texture_;
	Image image_;
};