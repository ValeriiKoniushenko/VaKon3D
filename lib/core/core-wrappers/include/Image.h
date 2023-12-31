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

#include "Gl.h"
#include "JsonPrintable.h"
#include "NotCopyableButMovable.h"
#include "glm/glm.hpp"

#include <filesystem>
#include <string>

class Image : Utils::NotCopyableButMovable, public JsonPrintable
{
public:
	enum class Channel
	{
		// Next values were taken from the stb_image.h documentation.
		None = 0,
		Grey = 1,
		GreyA = 2,
		RGB = 3,
		RGBA = 4
	};

public:
	[[nodiscard]] static std::string channelToString(Channel channel);
	[[nodiscard]] static GLenum convertChannelToGlChannel(Channel channel);

	explicit Image(std::filesystem::path path = "", Gl::Texture::Channel channel = Gl::Texture::Channel::SRGB);

	Image(Image&& obj) noexcept;
	Image& operator=(Image&& obj) noexcept;

	~Image() override;

	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;
	[[nodiscard]] glm::ivec2 getSize() const;
	[[nodiscard]] Channel getChannel() const;
	[[nodiscard]] unsigned char* data();
	[[nodiscard]] const unsigned char* data() const;
	void setInternalChannel(Gl::Texture::Channel channel);
	[[nodiscard]] Gl::Texture::Channel getInternalChannel() const;
	void loadImage(const std::filesystem::path&, bool isFlipVertically = true);
	void loadImage(std::filesystem::path&&, bool isFlipVertically = true);
	void loadToGpu(Gl::Texture::Target target = Gl::Texture::Target::Texture2D);
	void clear();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] nlohmann::json toJson() const override;

private:
	void init_();

private:
	unsigned char* data_{};
	int width_{}, height_{};
	Channel channel_ = Channel::None;
	Gl::Texture::Channel internalChannel_ = Gl::Texture::Channel::RGB;
	std::string name_;
};