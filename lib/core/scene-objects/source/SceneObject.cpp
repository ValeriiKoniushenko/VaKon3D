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

#include "SceneObject.h"

#include "WorldVariables.h"
#include "glm/gtx/transform.hpp"

#include <algorithm>

void SceneObject::setPosition(const glm::vec3& position)
{
	position_ = position;
	matricesAreDirty_ = true;
}

const glm::vec3& SceneObject::getPosition() const
{
	return position_;
}

glm::vec3& SceneObject::getPosition()
{
	return position_;
}

void SceneObject::moveForward(float offset)
{
	position_ += offset * getForwardVector();
	matricesAreDirty_ = true;
}

void SceneObject::moveRight(float offset)
{
	position_ += -offset * getRightVector();
	matricesAreDirty_ = true;
}

void SceneObject::moveUp(float offset)
{
	position_ += offset * getUpVector();
	matricesAreDirty_ = true;
}

void SceneObject::setRotation(glm::vec2 rotation)
{
	rotation_ = rotation;
	matricesAreDirty_ = true;
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
	while (rotation_.x >= 360.f)
	{
		rotation_.x -= 360.f;
	}
}

void SceneObject::rotate(glm::vec2 value)
{
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
	while (rotation_.x >= 360.f)
	{
		rotation_.x -= 360.f;
	}

	matricesAreDirty_ = true;
}

const glm::vec2& SceneObject::getRotation() const
{
	return rotation_;
}

glm::vec2& SceneObject::getRotation()
{
	return rotation_;
}

void SceneObject::setRotationX(float x)
{
	rotation_.x = x;
	matricesAreDirty_ = true;
	if (rotation_.x > maxPitch)
	{
		rotation_.x = maxPitch;
	}
	if (rotation_.x < -maxPitch)
	{
		rotation_.x = -maxPitch;
	}
}

void SceneObject::rotateX(float x)
{
	rotation_ += x;
	matricesAreDirty_ = true;
	while (rotation_.x >= 360.f)
	{
		rotation_.x -= 360.f;
	}
}

float SceneObject::getRotationX() const
{
	return rotation_.x;
}

void SceneObject::setRotationY(float y)
{
	rotation_.y = y;
	matricesAreDirty_ = true;
	while (rotation_.y >= 360.f)
	{
		rotation_.y -= 360.f;
	}
}

void SceneObject::rotateY(float y)
{
	rotation_.y += y;
	matricesAreDirty_ = true;
	while (rotation_.y >= 360.f)
	{
		rotation_.y -= 360.f;
	}
}

float SceneObject::getRotationY() const
{
	return rotation_.y;
}

void SceneObject::setMaxPitch(float value)
{
	maxPitch = value;
}

float SceneObject::getMaxPitch() const
{
	return maxPitch;
}

glm::vec3 SceneObject::getForwardVector() const
{
	auto r = glm::vec2(glm::radians(rotation_.x), glm::radians(rotation_.y));

	// clang-format off
	return glm::normalize(glm::vec3{
		cos(r.x) * -sin(r.y),
		sin(r.x),
		cos(r.x) * cos(r.y)
	});
	// clang-format on
}

glm::vec3 SceneObject::getUpVector() const
{
	auto r = glm::vec2(glm::radians(rotation_.x), glm::radians(rotation_.y));
	r.x += glm::radians(-90.f);

	// clang-format off
	return glm::normalize(glm::vec3{
		cos(r.x) * -sin(r.y),
		sin(r.x),
		cos(r.x) * cos(r.y)
	});
	// clang-format on
}

glm::vec3 SceneObject::getRightVector() const
{
	return glm::normalize(glm::cross(getForwardVector(), getUpVector()));
}

void SceneObject::recalculateMatrices()
{
	if (matricesAreDirty_)
	{
		cachedModelMatrix_ = glm::mat4(1.f);

		cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
		cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));

		cachedModelMatrix_ = glm::translate(cachedModelMatrix_, position_);

		onRecalculateMatrices.trigger();

		matricesAreDirty_ = false;
	}
}

void SceneObject::addImpulseForward(float value)
{
	if (speed_ < maxSpeed_)
	{
		impulse_.z += value / mass_ * std::get<float>(GetWorldVariables()["tick"]);
	}
}

void SceneObject::addImpulseRight(float value)
{
	if (speed_ < maxSpeed_)
	{
		impulse_.x += value / mass_ * std::get<float>(GetWorldVariables()["tick"]);
	}
}

void SceneObject::addImpulseUp(float value)
{
	if (speed_ < maxSpeed_)
	{
		impulse_.y += value / mass_ * std::get<float>(GetWorldVariables()["tick"]);
	}
}

void SceneObject::update()
{
	impulse_ *= airResistance;
	speed_ = glm::length(impulse_) * 1'000.f;

	moveForward(impulse_.z);
	moveRight(impulse_.x);
	moveUp(impulse_.y);
}

void SceneObject::setMass(float mass)
{
	mass_ = std::clamp(mass, SceneObject::minMass, SceneObject::maxMass);
}

float SceneObject::getMass() const
{
	return mass_;
}

void SceneObject::setMaxSpeed(float value)
{
	maxSpeed_ = value;
}

float SceneObject::getMaxSpeed() const
{
	return maxSpeed_;
}

float SceneObject::getCurrentSpeed() const
{
	return speed_;
}
