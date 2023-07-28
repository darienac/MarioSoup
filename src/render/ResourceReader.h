#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

class ResourceReader {
	private:
	public:
	enum ResourceType {
		Shader,
		Model,
		Texture
	};
	const static std::string readResource(ResourceType type, const std::string& path) {
		std::string fullPath = getFullPath(type, path);
		std::ifstream file(fullPath);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	const static std::string getFullPath(ResourceType type, std::string path) {
		switch (type) {
		case Shader:
			path = "shaders/" + path;
			break;
		case Model:
			path = "models/" + path;
			break;
		case Texture:
			path = "textures/" + path;
			break;
		}

		path = "res/" + path;
		return path;
	}
};
