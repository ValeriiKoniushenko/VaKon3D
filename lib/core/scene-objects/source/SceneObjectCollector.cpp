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

#include "SceneObjectCollector.h"

#include "SceneObject.h"

SceneObjectCollector& GetSceneObjectCollector()
{
	return SceneObjectCollector::instance();
}

void SceneObjectCollector::add(SceneObject* object)
{
	data_.push_back(object);
}

void SceneObjectCollector::remove(SceneObject* object)
{
	data_.erase(std::remove(data_.begin(), data_.end(), object), data_.end());
}

std::vector<SceneObject*>::iterator SceneObjectCollector::begin()
{
	return data_.begin();
}

std::vector<SceneObject*>::const_iterator SceneObjectCollector::begin() const
{
	return data_.cbegin();
}

std::vector<SceneObject*>::iterator SceneObjectCollector::end()
{
	return data_.end();
}

std::vector<SceneObject*>::const_iterator SceneObjectCollector::end() const
{
	return data_.cend();
}

SceneObject* SceneObjectCollector::at(std::size_t index)
{
	return data_.at(index);
}

SceneObject* SceneObjectCollector::at(const std::string& name)
{
	auto it = std::find_if(data_.begin(), data_.end(), [&name](SceneObject* obj) { return obj->getName() == name; });
	return it == data_.end() ? nullptr : *it;
}
