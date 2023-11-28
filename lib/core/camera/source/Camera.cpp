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

void Camera::rotate(glm::vec2 value)
{
	// SceneObject::rotate(value); - Don't do this

	value /= sensitive_;

	rotation_.y += value.x;
	rotation_.x += value.y;

	if (rotation_.x > maxPitch)
	{
		rotation_.x = maxPitch;
	}
	if (rotation_.x < -maxPitch)
	{
		rotation_.x = -maxPitch;
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

boost::property_tree::ptree Camera::toJson() const
{
	boost::property_tree::ptree tree;
	tree.put("", 1);

	return tree;
}
