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

Camera::Camera()
{
	setName("Camera");
	isReverseMatrixCalculating_ = true;
	onRecalculateMatrices.subscribe(
		[this]()
		{
			if (matricesAreDirty_)
			{
				auto windowSize = GetWindow().getSize();
				cachedProjMatrix_ = glm::perspective(
					glm::radians(fov_), static_cast<float>(windowSize.width) / static_cast<float>(windowSize.height), near_, far_);

				cachedCalculatedMatrix_ = cachedProjMatrix_ * cachedModelMatrix_;	 // cachedModelMatrix_ == view matrix

				matricesAreDirty_ = false;
			}
		});
}

void Camera::rotate(const glm::vec3& value_)
{
	// SceneObject::rotate(value); - Don't do this
	auto value = value_;
	value.x /= sensitive_.x;
	value.y /= sensitive_.y;

	rotation_.y += value.x;
	rotation_.x += value.y;

	if (rotation_.x > maxPitch_)
	{
		rotation_.x = maxPitch_;
	}
	if (rotation_.x < -maxPitch_)
	{
		rotation_.x = -maxPitch_;
	}

	while (rotation_.y >= 360.f)
	{
		rotation_.y -= 360.f;
	}

	matricesAreDirty_ = true;
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

void Camera::setSensitive(glm::vec2 value)
{
	sensitive_ = value;
}

glm::vec2 Camera::getSensitive() const
{
	return sensitive_;
}

nlohmann::json Camera::toJson() const
{
	auto json = SceneObject::toJson();

	json["cachedProjMatrix"] = {{cachedProjMatrix_[0].x, cachedProjMatrix_[0].y, cachedProjMatrix_[0].z, cachedProjMatrix_[0].w},
		{cachedProjMatrix_[1].x, cachedProjMatrix_[1].y, cachedProjMatrix_[1].z, cachedProjMatrix_[1].w},
		{cachedProjMatrix_[2].x, cachedProjMatrix_[2].y, cachedProjMatrix_[2].z, cachedProjMatrix_[2].w},
		{cachedProjMatrix_[3].x, cachedProjMatrix_[3].y, cachedProjMatrix_[3].z, cachedProjMatrix_[3].w}};
	json["cachedCalculatedMatrix"] = {
		{cachedCalculatedMatrix_[0].x, cachedCalculatedMatrix_[0].y, cachedCalculatedMatrix_[0].z, cachedCalculatedMatrix_[0].w},
		{cachedCalculatedMatrix_[1].x, cachedCalculatedMatrix_[1].y, cachedCalculatedMatrix_[1].z, cachedCalculatedMatrix_[1].w},
		{cachedCalculatedMatrix_[2].x, cachedCalculatedMatrix_[2].y, cachedCalculatedMatrix_[2].z, cachedCalculatedMatrix_[2].w},
		{cachedCalculatedMatrix_[3].x, cachedCalculatedMatrix_[3].y, cachedCalculatedMatrix_[3].z, cachedCalculatedMatrix_[3].w}};
	json["far"] = far_;
	json["fov"] = fov_;
	json["near"] = near_;
	json["sensitive"] = {sensitive_.x, sensitive_.y};

	return json;
}

glm::vec3 Camera::getPosition() const
{
	return -position_;
}

void Camera::setPosition(const glm::vec3& position)
{
	position_ = -position;
}