#ifndef Model3D_hpp
#define Model3D_hpp

#include "Mesh.hpp"

#include "tiny_obj_loader.h"
#include "stb_image.h"

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

		void LoadModel(std::string fileName);

		void LoadModel(std::string fileName, std::string basePath);

	private:
		std::vector<gps::Texture> loadedTextures;

		void ReadOBJ(std::string fileName, std::string basePath);

		gps::Texture LoadTexture(std::string path, std::string type);

		GLuint ReadTextureFromFile(const char *file_name);
	};
}

#endif /* Model3D_hpp */
