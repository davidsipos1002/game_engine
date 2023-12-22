#ifndef Model3D_hpp
#define Model3D_hpp

#include <object/Mesh.hpp>

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

		void LoadModel(std::string fileName);

		void Draw(gps::Shader *shaderProgram);

	private:
		std::vector<gps::Texture> loadedTextures;

		void ReadOBJ(std::string fileName, std::string basePath);

		gps::Texture LoadTexture(std::string path, std::string type);

		GLuint ReadTextureFromFile(const char *file_name);
	};
}

#endif /* Model3D_hpp */
