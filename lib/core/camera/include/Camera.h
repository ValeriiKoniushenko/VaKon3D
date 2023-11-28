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

#include "SceneObject.h"

class Camera : public SceneObject
{
public:
	Camera();

	void rotate(glm::vec2 value) override;

	[[nodiscard]] const glm::mat4& getMatrix();

	void setFov(float fov);
	[[nodiscard]] float getFov() const;

	void setNear(float value);
	[[nodiscard]] float getNear() const;

	void setFar(float value);
	[[nodiscard]] float getFar() const;

	void setSensitive(glm::vec2 value);
	[[nodiscard]] glm::vec2 getSensitive() const;

	[[nodiscard]] boost::property_tree::ptree toJson() const override;
	
protected:
	glm::vec2 sensitive_{2.f, 2.f};
	glm::mat4 cachedProjMatrix_ = glm::mat4(1.f);
	glm::mat4 cachedCalculatedMatrix_ = glm::mat4(1.f);
	float fov_ = 90.f;
	float far_ = 10'000.f;
	float near_ = 0.001f;
};