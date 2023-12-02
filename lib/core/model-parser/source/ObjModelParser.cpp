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

#include "ObjModelParser.h"

#include <iostream>

void ObjModelParser::setObjFileContent(const std::string& buffer)
{
	file_ = std::move(std::stringstream(buffer));

	parse();

	int i = 1;
}

std::pair<ObjModelParser::VertexData, std::string> ObjModelParser::getLine(std::string line) const
{
	if (line.size() < 2)	// max len of Vertex Data is 2
	{
		return {};
	}

	std::pair<VertexData, std::string> pair;
	int needShiftFor = 0;

	if (line.at(0) == 'v' && line.at(1) == 't')
	{
		needShiftFor = 2;
		pair.first = VertexData::Uv;
	}
	else if (line.at(0) == 'v' && line.at(1) == 'n')
	{
		needShiftFor = 2;
		pair.first = VertexData::Normal;
	}
	else if (line.at(0) == 'o')
	{
		needShiftFor = 1;
		pair.first = VertexData::ObjectName;
	}
	else if (line.at(0) == 'v')
	{
		needShiftFor = 1;
		pair.first = VertexData::Geometric;
	}
	else if (line.at(0) == 'p')
	{
		pair.first = VertexData::Parameter;
		needShiftFor = 1;
		throw std::runtime_error("Can't parse a mode. It has 'p' - parameter. This app still can't parse it correctly.");
	}
	else if (line.at(0) == 'f')
	{
		needShiftFor = 1;
		pair.first = VertexData::Face;
	}
	else
	{
		// throw std::runtime_error("Can't parse a mode. Was met unknown param");
	}

	needShiftFor += 1;	  // space

	for (std::size_t i = needShiftFor; i < line.size(); ++i)
	{
		pair.second.push_back(line.at(i));
	}

	return pair;
}

bool ObjModelParser::isIgnoreLine(const std::string& line) const
{
	return line.empty() || line.at(0) == '#' || line.at(0) == 's';
}

std::vector<ObjModelParser::ModelData> ObjModelParser::getModelData()
{
	return std::move(modelsData_);
}

void ObjModelParser::parse()
{
	while (!file_.eof())
	{
		std::string line;
		std::getline(file_, line);

		if (isIgnoreLine(line))
		{
			continue;
		}

		extrudeDataFromLine(getLine(std::move(line)));
	}

	convertFileDataToModels();
}

void ObjModelParser::extrudeDataFromLine(std::pair<VertexData, std::string> lineData)
{
	switch (lineData.first)
	{
		case VertexData::ObjectName:
		{
			fileData_.push_back({});
			fileData_.back().o = std::move(lineData.second);
			break;
		}
		case VertexData::Geometric:
		{
			glm::vec3 data;
			std::stringstream ss = std::move(std::stringstream(lineData.second));
			ss >> data.x;
			ss >> data.y;
			ss >> data.z;
			if (fileData_.empty())
			{
				throw std::runtime_error("Bad .obj file. The file was corrupted");
			}
			fileData_.back().v.push_back(data);
			break;
		}
		case VertexData::Uv:
		{
			glm::vec2 data;
			std::stringstream ss = std::move(std::stringstream(lineData.second));
			ss >> data.x;
			ss >> data.y;
			if (fileData_.empty())
			{
				throw std::runtime_error("Bad .obj file. The file was corrupted");
			}
			fileData_.back().vt.push_back(data);
			break;
		}
		case VertexData::Normal:
		{
			glm::vec3 data;
			std::stringstream ss = std::move(std::stringstream(lineData.second));
			ss >> data.x;
			ss >> data.y;
			ss >> data.z;
			if (fileData_.empty())
			{
				throw std::runtime_error("Bad .obj file. The file was corrupted");
			}
			fileData_.back().vn.push_back(data);
			break;
		}
		case VertexData::Face:
		{
			std::stringstream ss = std::move(std::stringstream(lineData.second));
			int i = 0;
			while (!ss.eof())
			{
				++i;
				if (i == 4)
				{
					throw std::runtime_error(
						"Error reading of the model. Please convert Quads to triangles. \n"
						"1. Open blender and open your model.\n"
						"2. Import wavefront .obj file in Blender and change the mode to Edit Mode.\n"
						"3. Now select all the meshes.\n"
						"4. Go to Mesh -> Face menu options and select convert Quads to triangles\n"
						"5. Export the current model again to wavefront .obj file");
				}
				glm::vec3 data;
				std::string token;

				std::getline(ss, token, '/');
				data.x = atof(token.c_str());

				std::getline(ss, token, '/');
				data.y = atof(token.c_str());

				std::getline(ss, token, ' ');
				auto dbg = ss.str();
				data.z = atof(token.c_str());

				if (fileData_.empty())
				{
					throw std::runtime_error("Bad .obj file. The file was corrupted");
				}
				fileData_.back().f.push_back(data);
			}
			break;
		}
	}
}

void ObjModelParser::convertFileDataToModels()
{
	for (std::size_t i = 0; i < fileData_.size(); ++i)
	{
		convertFileDataToModel(i);
	}
}

void ObjModelParser::convertFileDataToModel(std::size_t index)
{
	modelsData_.push_back({});

	auto& data = fileData_.at(index);
	modelsData_.back().name = data.o;

	for (auto face : data.f)
	{
		face -= 1;
		if (index != 0)
		{
			for (int i = index - 1; i >= 0; --i)
			{
				auto& prevData = fileData_.at(i);
				face.x -= prevData.v.size();
				face.y -= prevData.vt.size();
				face.z -= prevData.vn.size();
			}
		}
		TriangleVbo::Unit unit;

		if (face.x >= 0)
		{
			unit.position = data.v.at(face.x);
		}

		if (face.y >= 0)
		{
			unit.uv = data.vt.at(face.y);
		}

		if (face.z >= 0)
		{
			unit.normal = data.vn.at(face.z);
		}

		modelsData_.back().vertices_.push_back(unit);
	}
}
