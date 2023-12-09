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

#include "Color.h"
#include "glm/glm.hpp"

struct Lightning
{
	struct Ambient
	{
		GlColor3 lightColor = {1.f, 1.f, 1.f};
		glm::vec3 direction = {1.f, 0.7f, 0.5f};
		float maxDark = 0.02f;
	} ambient;

	struct Specular
	{
		GlColor3 lightColor = {1.f, 1.f, 1.f};
		glm::vec3 position = {1000.f, 1000.f, 1000.f};
		float intensity = 0.3f;
		int specularPow = 1024;
	} specular;

	struct Fog
	{
		Color4 color = {0, 0, 0, 0};
		float minDistance = 4000.f;
		float maxDistance = 8000.f;
	} fog;
};