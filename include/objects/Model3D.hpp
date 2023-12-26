#pragma once

#include <objects/Mesh.hpp>

#include <io/tiny_obj_loader.h>
#include <io/stb_image.h>
#include <graphics/Shader.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace gps
{

	class Model3D
	{

	public:
		std::unordered_map<std::string, Mesh*> meshes;

		~Model3D();

		void loadModel(const std::string &fileName);
		void draw(gps::Shader *shaderProgram);

	private:
		std::vector<gps::Texture> loadedTextures;

		void readOBJ(const std::string &fileName, const std::string &basePath);
		gps::Texture loadTexture(const std::string &path, const std::string &type);
		GLuint readTextureFromFile(const char *file_name);
	};
}
