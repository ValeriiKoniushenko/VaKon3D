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
#include "Lightning.h"
#include "Line.h"
#include "Triangle.h"
#include "TriangleVbo.h"
#include "Updateable.h"
#include "Vao.h"
#include "glm/glm.hpp"

class Camera;
class RayCast;
class ShaderPack;
class Texture2D;
class Texture;

class SceneObject : public JsonPrintable, public Updateable
{
public:
	inline static const float minMass = 0.0001f;
	inline static const float maxMass = 16'000'000.f;
	inline static const float airResistance = 0.95f;

	SceneObject();
	SceneObject(SceneObject&& other) = default;
	SceneObject& operator=(SceneObject&& other) = default;
	~SceneObject() override;

	/** brief: working in global coordinates */
	virtual void setPosition(const glm::vec3& position);

	/** brief: working in global coordinates */
	[[nodiscard]] virtual glm::vec3 getPosition() const;

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

	virtual void setRotation(const glm::vec3& rotation);
	virtual void rotate(const glm::vec3& value);
	[[nodiscard]] virtual const glm::vec3& getRotation() const;
	[[nodiscard]] virtual glm::vec3& getRotation();
	virtual void setRotationX(float x);
	virtual void rotateX(float x);
	[[nodiscard]] virtual float getRotationX() const;
	virtual void setRotationY(float y);
	virtual void rotateY(float y);
	[[nodiscard]] virtual float getRotationY() const;
	virtual void setRotationZ(float z);
	virtual void rotateZ(float z);
	[[nodiscard]] virtual float getRotationZ() const;

	virtual void setScale(const glm::vec3& value);
	virtual void scale(const glm::vec3& value);
	[[nodiscard]] virtual glm::vec3 getScale() const;

	virtual void setMaxPitch(float value);
	[[nodiscard]] virtual float getMaxPitch() const;

	[[nodiscard]] virtual glm::vec3 getForwardVector() const;
	[[nodiscard]] virtual glm::vec3 getUpVector() const;
	[[nodiscard]] virtual glm::vec3 getRightVector() const;

	void update() override;

	virtual void setMass(float mass);
	[[nodiscard]] float getMass() const;

	virtual void setMaxSpeed(float value);
	[[nodiscard]] float getMaxSpeed() const;
	[[nodiscard]] float getCurrentSpeed() const;

	virtual void draw(ShaderPack& shaderPack, const Lightning& lightning, Camera& camera);

	virtual void setOrigin(const glm::vec3& origin);
	[[nodiscard]] virtual const glm::vec3& getOrigin() const;

	virtual void setTexture(Texture2D& texture);
	virtual void setTexture(Texture& texture);
	[[nodiscard]] virtual Texture* getTexture();
	[[nodiscard]] virtual const Texture* getTexture() const;

	virtual void setSpecularTexture(Texture2D& texture);
	virtual void setSpecularTexture(Texture& texture);
	[[nodiscard]] virtual Texture* getSpecularTexture();
	[[nodiscard]] virtual const Texture* getSpecularTexture() const;

	void virtual setOutlineStatus(bool isEnable);
	void virtual toggleOutline();
	[[nodiscard]] virtual bool getOutlineStatus() const;

	[[nodiscard]] virtual bool isDrawCoordinateSystem() const;
	virtual void setDrawCoordinateSystem(bool isDraw);
	virtual void toggleDrawCoordinateSystem();

	virtual void setOutlineColor(const Color4& color);
	[[nodiscard]] virtual Color4 getOutlineColor() const;

	virtual void setOutlineSize(const glm::vec3& size);
	[[nodiscard]] virtual glm::vec3 getOutlineSize() const;

	virtual void setName(const std::string& name);
	[[nodiscard]] virtual const std::string& getName() const;
	[[nodiscard]] virtual std::string& getName();

	virtual void loadVertices(std::vector<TriangleVbo::Unit>& itWillBeMoved);

	virtual void setTextureRect(glm::vec2 rect);

	[[nodiscard]] nlohmann::json toJson() const override;

	[[nodiscard]] virtual std::optional<glm::vec3> isIntersectsWithRayCast(const RayCast& ray) const;

	void saveToCache();
	void loadFromCache();
	[[nodiscard]] bool checkAvailabilityInCache();

protected:
	virtual void recalculateMatrices();
	virtual void setVertices(){};
	virtual void tryDrawCoordinateSystem(ShaderPack& shaderPack, Camera& camera);
	virtual void tryDrawOutline(ShaderPack& shaderPack, Camera& camera);

	LambdaMulticastDelegate<void()> onRecalculateMatrices;

protected:
	std::string name_ = "None";
	glm::vec3 scale_ = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 origin_{};
	float speed_{};
	float maxSpeed_{2000.f};
	float mass_ = 1.f;
	glm::vec3 impulse_{};
	float maxPitch_ = 90.f;
	glm::vec3 position_{0.f, 0.f, 0.f};
	glm::vec3 rotation_{};
	bool matricesAreDirty_ = true;
	bool isDrawOutline_ = false;
	glm::mat4 cachedModelMatrix_ = glm::mat4(1.f);
	bool isReverseMatrixCalculating_ = false;
	Texture* diffuseTexture_ = nullptr;
	Texture* specularTexture_ = nullptr;
	TriangleVbo vbo_;
	Vao vao_;
	std::vector<TriangleVbo::Unit> triangles_;
	bool isDirtyTexture_ = false;
	bool verticesAreDirty_ = true;
	glm::vec3 outlineSize_ = glm::vec3(.1f);
	Color4 outlineColor_ = {247, 217, 17, 255};
	Line lineX_;
	Line lineY_;
	Line lineZ_;
	inline static const float lineWidth_ = 5.f;
	inline static const float lineSize_ = 150.f;
	bool isDrawSystemCoord_ = false;
};