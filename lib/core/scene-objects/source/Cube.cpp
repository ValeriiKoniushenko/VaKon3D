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

#include "Cube.h"

#include "Camera.h"
#include "Image.h"
#include "ShaderPack.h"
#include "Texture.h"

boost::property_tree::ptree Cube::toJson() const
{
	return boost::property_tree::ptree();
}

void Cube::setSize(float size)
{
	size_ = size;
	verticesAreDirty_ = true;
}

float Cube::getSize() const
{
	return size_;
}

void Cube::setVertices()
{
	triangles_.clear();
	triangles_.reserve(sidesCount * Triangle::verticesCount);

	using Unit = TriangleVbo::Unit;
	glm::vec2 textureSize = diffuseTexture_->getImage()->getSize();

	// front side
	triangles_.emplace_back(Unit{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, 0.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}});
	// back side
	triangles_.emplace_back(Unit{{0.f, 0.f, -size_}, {0.f, 0.f}, {0.f, 0.f, -1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, -size_}, {1.f, 1.f}, {0.f, 0.f, -1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, -size_}, {1.f, 0.f}, {0.f, 0.f, -1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, 0.f, -size_}, {0.f, 0.f}, {0.f, 0.f, -1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, -size_}, {0.f, 1.f}, {0.f, 0.f, -1.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, -size_}, {1.f, 1.f}, {0.f, 0.f, -1.f}, {textureSize.y, textureSize.y}});
	// left side
	triangles_.emplace_back(Unit{{0.f, 0.f, 0.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, 0.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, 0.f, -size_}, {0.f, 0.f}, {-1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, 0.f, -size_}, {0.f, 0.f}, {-1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, 0.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, -size_}, {1.f, 0.f}, {-1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	// right side
	triangles_.emplace_back(Unit{{size_, 0.f, 0.f}, {1.f, 0.f}, {1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, -size_}, {0.f, 0.f}, {1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, 0.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, -size_}, {0.f, 0.f}, {1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, -size_}, {1.f, 0.f}, {1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, 0.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}, {textureSize.x, textureSize.y}});
	// top side
	triangles_.emplace_back(Unit{{0.f, size_, 0.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, -size_}, {1.f, 1.f}, {0.f, 1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, -size_}, {0.f, 1.f}, {0.f, 1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, -size_}, {1.f, 1.f}, {0.f, 1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, size_, 0.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, size_, 0.f}, {1.f, 0.f}, {0.f, 1.f, 0.f}, {textureSize.x, textureSize.y}});
	// bottom side
	triangles_.emplace_back(Unit{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, 0.f, -size_}, {0.f, 1.f}, {0.f, -1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, -size_}, {1.f, 1.f}, {0.f, -1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, -size_}, {1.f, 1.f}, {0.f, -1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{size_, 0.f, 0.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}, {textureSize.x, textureSize.y}});
	triangles_.emplace_back(Unit{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}, {textureSize.x, textureSize.y}});
}

void Cube::update()
{
	SceneObject::update();
}
