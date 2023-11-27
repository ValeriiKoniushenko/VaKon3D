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

#include "Camera.h"

#include "Window.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

void Camera::setPosition(const glm::vec3& position)
{
	position_ = position;
	matrixIsDirty_ = true;
}

const glm::vec3& Camera::getPosition() const
{
	return position_;
}

glm::vec3& Camera::getPosition()
{
	return position_;
}

void Camera::move(const glm::vec3& offset)
{
	position_ += offset;
	matrixIsDirty_ = true;
}

void Camera::setRotation(const glm::vec2& rotation)
{
	rotation_ = rotation;
	matrixIsDirty_ = true;
	while (rotation_.x >= 360.f)
	{
		rotation_.x -= 360.f;
	}
	while (rotation_.y >= 360.f)
	{
		rotation_.y -= 360.f;
	}
}

const glm::vec2& Camera::getRotation() const
{
	return rotation_;
}

glm::vec2& Camera::getRotation()
{
	return rotation_;
}

void Camera::setRotationX(float x)
{
	rotation_.x = x;
	matrixIsDirty_ = true;
	while (rotation_.x >= 360.f)
	{
		rotation_.x -= 360.f;
	}
}

void Camera::rotateX(float x)
{
	rotation_ += x;
	matrixIsDirty_ = true;
	while (rotation_.x >= 360.f)
	{
		rotation_.x -= 360.f;
	}
}

float Camera::getRotationX() const
{
	return rotation_.x;
}

void Camera::setRotationY(float y)
{
	rotation_.y = y;
	matrixIsDirty_ = true;
	while (rotation_.y >= 360.f)
	{
		rotation_.y -= 360.f;
	}
}

void Camera::rotateY(float y)
{
	rotation_.y += y;
	matrixIsDirty_ = true;
	while (rotation_.y >= 360.f)
	{
		rotation_.y -= 360.f;
	}
}

float Camera::getRotationY() const
{
	return rotation_.y;
}

const glm::mat4& Camera::getMatrix()
{
	recalculateMatrices();
	return cachedCalculatedMatrix_;
}

void Camera::setFov(float fov)
{
	fov_ = fov;
}

float Camera::getFov() const
{
	return fov_;
}

void Camera::setNear(float value)
{
	near_ = value;
}

float Camera::getNear() const
{
	return near_;
}

void Camera::setFar(float value)
{
	far_ = value;
}

float Camera::getFar() const
{
	return far_;
}

void Camera::recalculateMatrices()
{
	auto windowSize = GetWindow().getSize();
	if (cachedWidth_ != windowSize.width || cachedHeight_ != windowSize.height || matrixIsDirty_)
	{
		cachedProjMatrix_ = glm::perspective(
			glm::radians(fov_), static_cast<float>(windowSize.width) / static_cast<float>(windowSize.height), near_, far_);

		matrixIsDirty_ = true;
		cachedWidth_ = windowSize.width;
		cachedHeight_ = windowSize.height;
	}

	if (matrixIsDirty_)
	{
		cachedViewMatrix_ = glm::mat4(1.f);

		cachedViewMatrix_ = glm::translate(cachedViewMatrix_, position_);

		cachedViewMatrix_ = glm::rotate(cachedViewMatrix_, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
		cachedViewMatrix_ = glm::rotate(cachedViewMatrix_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));

		cachedCalculatedMatrix_ = cachedProjMatrix_ * cachedViewMatrix_;

		matrixIsDirty_ = false;
	}
}
