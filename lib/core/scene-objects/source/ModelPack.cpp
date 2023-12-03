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

#include "ModelPack.h"

#include "ObjModelParser.h"
#include "SceneObjectCollector.h"
#include "UtilsFunctions.h"

boost::property_tree::ptree ModelPack::toJson() const
{
	return boost::property_tree::ptree();
}

void ModelPack::loadFromFile(const std::filesystem::path& path)
{
	requireValidFilePath(path);

	ObjModelParser parser;
	parser.setObjFileContent(Utils::getFileContent(path));

	auto md = std::move(parser.getModelData());
	for (auto& el : md)
	{
		SceneObject sceneObject;
		sceneObject.loadVertices(el.vertices_);
		sceneObject.setName(el.name);
		models_.emplace(el.name, std::move(sceneObject));
		GetSceneObjectCollector().add(&models_.find(el.name)->second);
	}
}

bool ModelPack::validateFileFormat(const std::filesystem::path& path) const
{
	return std::find(permissibleFileFormats.begin(), permissibleFileFormats.end(), path.extension()) !=
		   permissibleFileFormats.end();
}

void ModelPack::requireValidFilePath(const std::filesystem::path& path) const
{
	if (!validateFileFormat(path))
	{
		throw std::runtime_error("Unsupported file format. This version of the engine supports only the next types: " +
								 Utils::toString(permissibleFileFormats));
	}

	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Can't find such file.");
	}
}

SceneObject& ModelPack::getModel(const std::string& name)
{
	return models_.at(name);
}

const SceneObject& ModelPack::getModel(const std::string& name) const
{
	return models_.at(name);
}

SceneObject& ModelPack::operator[](const std::string& name)
{
	return models_.at(name);
}

const SceneObject& ModelPack::operator[](const std::string& name) const
{
	return models_.at(name);
}

std::unordered_map<std::string, SceneObject>::iterator ModelPack::begin()
{
	return models_.begin();
}

std::unordered_map<std::string, SceneObject>::const_iterator ModelPack::begin() const
{
	return models_.cbegin();
}

std::unordered_map<std::string, SceneObject>::iterator ModelPack::end()
{
	return models_.end();
}

std::unordered_map<std::string, SceneObject>::const_iterator ModelPack::end() const
{
	return models_.cend();
}
