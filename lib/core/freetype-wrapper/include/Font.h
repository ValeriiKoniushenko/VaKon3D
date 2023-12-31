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

#include "CopyableAndMoveable.h"
#include "FreeTypeLibrary.h"
#include "Gl.h"
#include "Texture.h"
#include "glm/glm.hpp"

#include <filesystem>
#include <unordered_map>

class Font : Utils::CopyableAndMoveable
{
public:
	struct Character
	{
		Texture texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	inline static constexpr float defaultRenderSize = 500.f;

	Font() = default;
	explicit Font(const std::filesystem::path& path);
	~Font() override;
	void loadFromFile(const std::filesystem::path& path);
	void destroy();
	[[nodiscard]] const Character& getCharacter(GLchar ch) const;

private:
	std::unordered_map<GLchar, Character> characters_;
	FT_Face face{};
	std::string fontName_;
};