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

struct GlColor4
{
	float r{};
	float g{};
	float b{};
	float a{1.f};
};

struct Color4
{
	unsigned char r{};
	unsigned char g{};
	unsigned char b{};
	unsigned char a{255};
};

inline GlColor4 toGlColor4(const Color4& color)
{
	return {.r = static_cast<float>(color.r) / 255.f,
		.g = static_cast<float>(color.g) / 255.f,
		.b = static_cast<float>(color.b) / 255.f,
		.a = static_cast<float>(color.a) / 255.f};
}

inline Color4 toColor4(const GlColor4& color)
{
	return {.r = static_cast<unsigned char>(color.r * 255),
		.g = static_cast<unsigned char>(color.g * 255),
		.b = static_cast<unsigned char>(color.b * 255),
		.a = static_cast<unsigned char>(color.a * 255)};
}

struct GlColor3
{
	float r{};
	float g{};
	float b{};
};

struct Color3
{
	unsigned char r{};
	unsigned char g{};
	unsigned char b{};
};

inline GlColor3 toGlColor3(const Color3& color)
{
	return {.r = static_cast<float>(color.r) / 255.f,
		.g = static_cast<float>(color.g) / 255.f,
		.b = static_cast<float>(color.b) / 255.f};
}

inline Color3 toColor3(const GlColor3& color)
{
	return {.r = static_cast<unsigned char>(color.r * 255),
		.g = static_cast<unsigned char>(color.g * 255),
		.b = static_cast<unsigned char>(color.b * 255)};
}