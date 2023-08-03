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
		Texture,
		World
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
			path = "res/shaders/" + path;
			break;
		case Model:
			path = "res/models/" + path;
			break;
		case Texture:
			path = "res/textures/" + path;
			break;
		case World:
			path = "worlds/" + path;
		}

		return path;
	}
};
