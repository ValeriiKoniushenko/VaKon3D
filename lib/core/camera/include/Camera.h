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

#include "NotCopyableButMovable.h"
#include "glm/glm.hpp"

class Camera : public Utils::NotCopyableButMovable
{
public:
	void setPosition(const glm::vec3& position);
	[[nodiscard]] const glm::vec3& getPosition() const;
	[[nodiscard]] glm::vec3& getPosition();
	void move(const glm::vec3& offset);

	void setRotation(const glm::vec2& rotation);
	[[nodiscard]] const glm::vec2& getRotation() const;
	[[nodiscard]] glm::vec2& getRotation();
	void setRotationX(float x);
	void rotateX(float x);
	[[nodiscard]] float getRotationX() const;
	void setRotationY(float y);
	void rotateY(float y);
	[[nodiscard]] float getRotationY() const;

	[[nodiscard]] const glm::mat4& getMatrix();

	void setFov(float fov);
	[[nodiscard]] float getFov() const;

	void setNear(float value);
	[[nodiscard]] float getNear() const;

	void setFar(float value);
	[[nodiscard]] float getFar() const;

private:
	void recalculateMatrices();

private:
	glm::mat4 cachedViewMatrix_ = glm::mat4(1.f);
	glm::mat4 cachedProjMatrix_ = glm::mat4(1.f);
	float fov_ = 90.f;
	float far_ = 10'000.f;
	float near_ = 0.001f;
	glm::vec3 position_{0.f, 0.f, -100.f};
	glm::vec2 rotation_{};
	bool matrixIsDirty_ = true;
	glm::mat4 cachedCalculatedMatrix_ = glm::mat4(1.f);
	int cachedWidth_ = 0.f;
	int cachedHeight_ = 0.f;
};