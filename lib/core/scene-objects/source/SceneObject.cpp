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

#include "Camera.h"
#include "Image.h"
#include "Raycast.h"
#include "SceneObjectCollector.h"
#include "ShaderPack.h"
#include "Texture.h"
#include "WorldVariables.h"
#include "glad.h"
#include "glm/gtx/transform.hpp"

#include <algorithm>

SceneObject::SceneObject()
{
	GetSceneObjectCollector().add(this);
}

SceneObject::~SceneObject()
{
	GetSceneObjectCollector().remove(this);
}

void SceneObject::setPosition(const glm::vec3& position)
{
	position_ = position;
	matricesAreDirty_ = true;
}

glm::vec3 SceneObject::getPosition() const
{
	return position_ + origin_;
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

void SceneObject::setRotation(const glm::vec3& rotation)
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
	while (rotation_.z >= 360.f)
	{
		rotation_.z -= 360.f;
	}
}

void SceneObject::rotate(const glm::vec3& value)
{
	rotation_.y += value.x;
	rotation_.x += value.y;
	rotation_.z += value.z;

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
	while (rotation_.z >= 360.f)
	{
		rotation_.z -= 360.f;
	}

	matricesAreDirty_ = true;
}

const glm::vec3& SceneObject::getRotation() const
{
	return rotation_;
}

glm::vec3& SceneObject::getRotation()
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

void SceneObject::setRotationZ(float z)
{
	rotation_.z = z;
	matricesAreDirty_ = true;
	while (rotation_.z >= 360.f)
	{
		rotation_.z -= 360.f;
	}
}

void SceneObject::rotateZ(float z)
{
	rotation_.z += z;
	matricesAreDirty_ = true;
	while (rotation_.z >= 360.f)
	{
		rotation_.z -= 360.f;
	}
}

float SceneObject::getRotationZ() const
{
	return rotation_.z;
}

void SceneObject::setScale(const glm::vec3& value)
{
	scale_ = value;
	matricesAreDirty_ = true;
}

void SceneObject::scale(const glm::vec3& value)
{
	scale_ += value;
	matricesAreDirty_ = true;
}

glm::vec3 SceneObject::getScale() const
{
	return scale_;
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

		if (!isReverseMatrixCalculating_)
		{
			cachedModelMatrix_ = glm::translate(cachedModelMatrix_, position_);

			cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
			cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
			cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
			cachedModelMatrix_ = glm::translate(cachedModelMatrix_, -origin_);

			cachedModelMatrix_ = glm::translate(cachedModelMatrix_, origin_);
			cachedModelMatrix_ = glm::scale(cachedModelMatrix_, scale_);
			cachedModelMatrix_ = glm::translate(cachedModelMatrix_, -origin_);
		}
		else
		{
			cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
			cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
			cachedModelMatrix_ = glm::rotate(cachedModelMatrix_, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));

			cachedModelMatrix_ = glm::translate(cachedModelMatrix_, position_);
			cachedModelMatrix_ = glm::translate(cachedModelMatrix_, origin_);
			cachedModelMatrix_ = glm::scale(cachedModelMatrix_, scale_);
		}

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

	recalculateMatrices();

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

void SceneObject::setOrigin(const glm::vec3& origin)
{
	origin_ = origin;
	matricesAreDirty_ = true;
}

const glm::vec3& SceneObject::getOrigin() const
{
	return origin_;
}

void SceneObject::draw(ShaderPack& shaderPack, const Lightning& lightning, Camera& camera)
{
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	recalculateMatrices();

	auto& shader = shaderPack["triangle"];
	shader.use();

	if (diffuseTexture_)
	{
		Gl::Texture::active(0);
		diffuseTexture_->bind();
	}
	if (specularTexture_)
	{
		Gl::Texture::active(1);
		specularTexture_->bind();
	}
	if (isDirtyTexture_)
	{
		if (diffuseTexture_)
		{
			Gl::Texture::active(0);
			diffuseTexture_->bind();
			diffuseTexture_->loadToGpu();
		}
		if (specularTexture_)
		{
			Gl::Texture::active(1);
			specularTexture_->bind();
			specularTexture_->loadToGpu();
		}
		isDirtyTexture_ = false;
	}
	if (!vao_.isGenerated())
	{
		vao_.generate();
	}
	if (!vbo_.isGenerated())
	{
		vbo_.generate();
	}

	vao_.bind();
	vbo_.bind();

	if (verticesAreDirty_)
	{
		setVertices();
		vbo_.data(triangles_);
		verticesAreDirty_ = false;
	}

	Gl::Vao::vertexAttribPointer(1, 3, Gl::Type::Float, false, 10 * sizeof(float), nullptr);
	Gl::Vao::vertexAttribPointer(2, 2, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(3, 3, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(4, 2, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(8 * sizeof(float)));

	shader.uniform("uProjectionAndView", false, camera.getMatrix());
	shader.uniform("uModel", false, cachedModelMatrix_);

	shader.uniform("uAmbientLightColor", lightning.ambient.lightColor);
	shader.uniform("uAmbientLightDirection", lightning.ambient.direction);
	shader.uniform("uAmbientLightMaxDark", lightning.ambient.maxDark);

	shader.uniform("uSpecularColor", lightning.specular.lightColor);
	shader.uniform("uSpecularPosition", lightning.specular.position);
	shader.uniform("uSpecularIntensity", lightning.specular.intensity);
	shader.uniform("uSpecularPow", lightning.specular.specularPow);
	shader.uniform("uViewPosition", camera.getPosition());
	shader.uniform("uFogColor", toGlColor4(lightning.fog.color));
	shader.uniform("uFogMinDistance", lightning.fog.minDistance);
	shader.uniform("uFogMaxDistance", lightning.fog.maxDistance);

	shader.uniform("uTexture", 0);
	shader.uniform("uSpecularTexture", 1);
	if (specularTexture_)
	{
		shader.uniform("uIsHasSpecularTexture", true);
	}
	else
	{
		shader.uniform("uIsHasSpecularTexture", false);
	}

	if (diffuseTexture_)
	{
		shader.uniform("uAtlasSize", diffuseTexture_->getImage()->getSize());
	}

	Gl::drawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangles_.size()) * Triangle::verticesCount);

	if (diffuseTexture_)
	{
		diffuseTexture_->unbind();
	}
	if (specularTexture_)
	{
		specularTexture_->unbind();
	}

	tryDrawOutline(shaderPack, camera);
	tryDrawCoordinateSystem(shaderPack, camera);
}

void SceneObject::tryDrawOutline(ShaderPack& shaderPack, Camera& camera)
{
	if (!isDrawOutline_)
	{
		return;
	}

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDisable(GL_CULL_FACE);

	scale(outlineSize_);
	recalculateMatrices();

	auto& shader = shaderPack["outline"];
	shader.use();
	if (diffuseTexture_)
	{
		diffuseTexture_->bind();
		if (isDirtyTexture_)
		{
			diffuseTexture_->loadToGpu();
			isDirtyTexture_ = false;
		}
	}
	if (!vao_.isGenerated())
	{
		vao_.generate();
	}
	if (!vbo_.isGenerated())
	{
		vbo_.generate();
	}

	vao_.bind();
	vbo_.bind();

	if (verticesAreDirty_)
	{
		setVertices();
		vbo_.data(triangles_);
		verticesAreDirty_ = false;
	}

	Gl::Vao::vertexAttribPointer(1, 3, Gl::Type::Float, false, 10 * sizeof(float), nullptr);
	Gl::Vao::vertexAttribPointer(2, 2, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(3, 3, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));
	Gl::Vao::vertexAttribPointer(4, 2, Gl::Type::Float, false, 10 * sizeof(float), reinterpret_cast<void*>(8 * sizeof(float)));

	shader.uniform("uProjectionAndView", false, camera.getMatrix());
	shader.uniform("uModel", false, cachedModelMatrix_);
	shader.uniform("uOutlineColor", toGlColor4(outlineColor_));

	Gl::drawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangles_.size()) * Triangle::verticesCount);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_CULL_FACE);
	glClear(GL_STENCIL_BUFFER_BIT);

	scale(-outlineSize_);
}

void SceneObject::setTexture(Texture& texture)
{
	diffuseTexture_ = &texture;
	isDirtyTexture_ = true;
}

Texture* SceneObject::getTexture()
{
	return diffuseTexture_;
}

const Texture* SceneObject::getTexture() const
{
	return diffuseTexture_;
}

void SceneObject::setSpecularTexture(Texture& texture)
{
	specularTexture_ = &texture;
	isDirtyTexture_ = true;
}

Texture* SceneObject::getSpecularTexture()
{
	return specularTexture_;
}

const Texture* SceneObject::getSpecularTexture() const
{
	return specularTexture_;
}

void SceneObject::setOutlineStatus(bool isEnable)
{
	isDrawOutline_ = isEnable;
}

bool SceneObject::getOutlineStatus() const
{
	return isDrawOutline_;
}

void SceneObject::setOutlineColor(const Color4& color)
{
	outlineColor_ = color;
}

Color4 SceneObject::getOutlineColor() const
{
	return outlineColor_;
}

void SceneObject::setOutlineSize(const glm::vec3& size)
{
	outlineSize_ = size;
}

glm::vec3 SceneObject::getOutlineSize() const
{
	return outlineSize_;
}

void SceneObject::setName(const std::string& name)
{
	name_ = name;
}

const std::string& SceneObject::getName() const
{
	return name_;
}

std::string& SceneObject::getName()
{
	return name_;
}

void SceneObject::loadVertices(std::vector<TriangleVbo::Unit>& itWillBeMoved)
{
	triangles_ = std::move(itWillBeMoved);
}

nlohmann::json SceneObject::toJson() const
{
	return {};
}

void SceneObject::setTextureRect(glm::vec2 rect)
{
	for (auto& el : triangles_)
	{
		el.textureRect = rect;
	}
}

void SceneObject::tryDrawCoordinateSystem(ShaderPack& shaderPack, Camera& camera)
{
	if (!isDrawSystemCoord_)
	{
		return;
	}

	auto& shader = shaderPack["line"];
	shader.use();

	shader.uniform("uViewPosition", camera.getPosition());

	lineX_.setWidth(lineWidth_);
	lineX_.setColor({0, 42, 255});
	glm::vec3 lineX = glm::vec3(lineSize_, 0.f, 0.f);
	glm::mat4 lineXModel = glm::mat4(1.f);
	lineXModel = glm::rotate(lineXModel, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
	lineXModel = glm::rotate(lineXModel, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
	lineXModel = glm::rotate(lineXModel, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
	lineX = glm::mat3(lineXModel) * lineX;
	lineX_.setStartAndEndPoint(position_, position_ + lineX);

	lineY_.setWidth(lineWidth_);
	lineY_.setColor({255, 0, 0});
	glm::vec3 lineY = glm::vec3(0.f, lineSize_, 0.f);
	glm::mat4 lineYModel = glm::mat4(1.f);
	lineYModel = glm::rotate(lineYModel, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
	lineYModel = glm::rotate(lineYModel, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
	lineYModel = glm::rotate(lineYModel, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
	lineY = glm::mat3(lineYModel) * lineY;
	lineY_.setStartAndEndPoint(position_, position_ + lineY);

	lineZ_.setWidth(lineWidth_);
	lineZ_.setColor({0, 255, 42});
	glm::vec3 lineZ = glm::vec3(0.f, 0.f, lineSize_);
	glm::mat4 lineZModel = glm::mat4(1.f);
	lineZModel = glm::rotate(lineZModel, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
	lineZModel = glm::rotate(lineZModel, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
	lineZModel = glm::rotate(lineZModel, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
	lineZ = glm::mat3(lineZModel) * lineZ;
	lineZ_.setStartAndEndPoint(position_, position_ + lineZ);

	glDepthFunc(GL_ALWAYS);
	lineX_.draw(shaderPack, camera);
	lineY_.draw(shaderPack, camera);
	lineZ_.draw(shaderPack, camera);
	glDepthFunc(GL_LESS);
}

void SceneObject::toggleOutline()
{
	isDrawOutline_ = !isDrawOutline_;
}

bool SceneObject::isDrawCoordinateSystem() const
{
	return isDrawSystemCoord_;
}

void SceneObject::setDrawCoordinateSystem(bool isDraw)
{
	isDrawSystemCoord_ = isDraw;
}

void SceneObject::toggleDrawCoordinateSystem()
{
	isDrawSystemCoord_ = !isDrawSystemCoord_;
}

std::optional<glm::vec3> SceneObject::isIntersectsWithRayCast(const RayCast& ray) const
{
	auto signedTetraVolume = [](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
	{ return glm::sign(glm::dot(glm::cross(b - a, c - a), d - a) / 6.0); };

	glm::vec3 points[3]{};
	int i = 0;
	for (auto& unit : triangles_)
	{
		points[i++] = cachedModelMatrix_ * glm::vec4(unit.position, 1.f);
		if (i == 3)
		{
			i = 0;

			auto s1 = signedTetraVolume(ray.getLine().first, points[0], points[1], points[2]);
			auto s2 = signedTetraVolume(ray.getLine().second, points[0], points[1], points[2]);

			if (s1 != s2)
			{
				auto s3 = signedTetraVolume(ray.getLine().first, ray.getLine().second, points[0], points[1]);
				auto s4 = signedTetraVolume(ray.getLine().first, ray.getLine().second, points[1], points[2]);
				auto s5 = signedTetraVolume(ray.getLine().first, ray.getLine().second, points[2], points[0]);
				if (s3 == s4 && s4 == s5)
				{
					auto n = glm::cross(points[1] - points[0], points[2] - points[0]);
					auto t = -glm::dot(ray.getLine().first, n - points[0]) /
							 glm::dot(ray.getLine().first, ray.getLine().second - ray.getLine().first);
					return std::make_optional(ray.getLine().first + t * (ray.getLine().second - ray.getLine().first));
				}
			}
		}
	}

	return {};
}