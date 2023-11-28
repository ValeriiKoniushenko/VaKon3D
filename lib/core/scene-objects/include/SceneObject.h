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

#include "Delegate.h"
#include "JsonPrintable.h"
#include "Updateable.h"
#include "glm/glm.hpp"

class SceneObject : public JsonPrintable, public Updateable
{
public:
	inline static const float minMass = 0.0001f;
	inline static const float maxMass = 16'000'000.f;
	inline static const float airResistance = 0.999f;

	/** brief: working in global coordinates */
	virtual void setPosition(const glm::vec3& position);

	/** brief: working in global coordinates */
	[[nodiscard]] virtual const glm::vec3& getPosition() const;

	/** brief: working in global coordinates */
	[[nodiscard]] virtual glm::vec3& getPosition();

	/** brief: working in local coordinates */
	virtual void moveForward(float offset);

	/** brief: working in local coordinates */
	virtual void moveRight(float offset);

	/** brief: working in local coordinates */
	virtual void moveUp(float offset);

	/** brief: working in local coordinates */
	virtual void addImpulseForward(float value);

	/** brief: working in local coordinates */
	virtual void addImpulseRight(float value);

	/** brief: working in local coordinates */
	virtual void addImpulseUp(float value);

	virtual void setRotation(glm::vec2 rotation);
	virtual void rotate(glm::vec2 value);
	[[nodiscard]] virtual const glm::vec2& getRotation() const;
	[[nodiscard]] virtual glm::vec2& getRotation();
	virtual void setRotationX(float x);
	virtual void rotateX(float x);
	[[nodiscard]] virtual float getRotationX() const;
	virtual void setRotationY(float y);
	virtual void rotateY(float y);
	[[nodiscard]] virtual float getRotationY() const;

	virtual void setMaxPitch(float value);
	[[nodiscard]] virtual float getMaxPitch() const;

	[[nodiscard]] virtual glm::vec3 getForwardVector() const;
	[[nodiscard]] virtual glm::vec3 getUpVector() const;
	[[nodiscard]] virtual glm::vec3 getRightVector() const;

	void update() override;

	void setMass(float mass);
	[[nodiscard]] float getMass() const;

protected:
	void recalculateMatrices();

	LambdaMulticastDelegate<void()> onRecalculateMatrices;

protected:
	float mass_ = 1000.f;
	glm::vec3 impulse_{};
	float maxPitch = 90.f;
	glm::vec3 position_{0.f, 0.f, -100.f};
	glm::vec2 rotation_{};
	bool matricesAreDirty_ = true;
	glm::mat4 cachedModelMatrix_ = glm::mat4(1.f);
};