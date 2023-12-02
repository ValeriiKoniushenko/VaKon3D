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

#include "SceneObject.h"

#include <filesystem>
#include <unordered_map>

class ModelPack : public Utils::CopyableAndMoveable, public JsonPrintable
{
public:
	inline static const std::vector<std::string> permissibleFileFormats = {".obj"};
	boost::property_tree::ptree toJson() const override;
	void loadFromFile(const std::filesystem::path& path);

	[[nodiscard]] SceneObject& getModel(const std::string& name);
	[[nodiscard]] const SceneObject& getModel(const std::string& name) const;
	[[nodiscard]] SceneObject& operator[](const std::string& name);
	[[nodiscard]] const SceneObject& operator[](const std::string& name) const;
	[[nodiscard]] std::unordered_map<std::string, SceneObject>::iterator begin();
	[[nodiscard]] std::unordered_map<std::string, SceneObject>::const_iterator begin() const;
	[[nodiscard]] std::unordered_map<std::string, SceneObject>::iterator end();
	[[nodiscard]] std::unordered_map<std::string, SceneObject>::const_iterator end() const;

private:
	[[nodiscard]] bool validateFileFormat(const std::filesystem::path& path) const;
	void requireValidFilePath(const std::filesystem::path& path) const;

protected:
	std::unordered_map<std::string, SceneObject> models_;
};