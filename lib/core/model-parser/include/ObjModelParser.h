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

#include "NotCopyableButMovable.h"
#include "TriangleVbo.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class ObjModelParser final : public Utils::NotCopyableButMovable
{
public:
	enum class VertexData
	{
		Geometric,	  // v
		Parameter,	  // p
		Face,		  // f
		Uv,			  // vt
		Normal,		  // vn
		ObjectName	  // o
	};
	inline static const std::unordered_map<VertexData, std::string> vertexData = {{VertexData::Geometric, "v"},
		{VertexData::Parameter, "p"}, {VertexData::Face, "f"}, {VertexData::Uv, "vt"}, {VertexData::Normal, "vn"},
		{VertexData::ObjectName, "o"}};

	struct ModelData
	{
		std::string name;
		std::vector<TriangleVbo::Unit> vertices_;
	};

	void setObjFileContent(const std::string& buffer);
	[[nodiscard]] std::vector<ModelData> getModelData();

private:
	std::pair<VertexData, std::string> getLine(std::string line) const;
	[[nodiscard]] bool isIgnoreLine(const std::string& line) const;
	void parse();
	void extrudeDataFromLine(std::pair<VertexData, std::string> lineData);
	void convertFileDataToModels();
	void convertFileDataToModel(std::size_t index);

	struct FileData
	{
		std::vector<glm::vec3> v;
		std::vector<glm::vec2> vt;
		std::vector<glm::vec3> vn;
		std::vector<glm::ivec3> f;
		std::string o;
	};

private:
	std::stringstream file_;
	std::vector<ModelData> modelsData_;
	std::vector<FileData> fileData_;
};