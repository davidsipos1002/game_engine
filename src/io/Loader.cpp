#include <io/Loader.hpp>

#define UUID_SYSTEM_GENERATOR
#include <io/uuid.h>
#include <io/stb_image.h>

namespace gps
{
    Loader::Loader() {}

    Loader::~Loader()
    {
        for (auto &shaderPair : shaders)
            delete shaderPair.second;
        for (auto &entityPair : entities)
            delete entityPair.second;
        for (auto &modelPair : models)
            delete modelPair.second;
        for (auto &skyBox : skyBoxes)
            delete skyBox;
        for (auto &particleTexture : particleTextures)
            delete particleTexture;
    }

    Entity *Loader::__loadEntity(const std::string &filename, std::string &uuid)
    {
        Model3D *model = nullptr;
        if (models.find(filename) == models.end())
        {
            model = new Model3D();
            model->loadModel(filename);
            models[filename] = model;
        }
        else
            model = models.at(filename);
        uuid = uuids::to_string(uuids::uuid_system_generator{}());
        Entity *entity = new Entity(model);
        entities[uuid] = entity;
        return entity;
    }

    Entity *Loader::loadEntity(const std::string &filename)
    {
        std::string uuid;
        return __loadEntity(filename, uuid);
    }

    Entity *Loader::loadEntity(const std::string &filename, std::string &uuid)
    {
        return __loadEntity(filename, uuid);
    }

    Entity *Loader::getEntity(const std::string &uuid)
    {
        return entities[uuid];
    }

    Shader *Loader::__loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string &uuid)
    {
        uuid = uuids::to_string(uuids::uuid_system_generator{}());
        Shader *shader = new Shader();
        shader->loadShader(vertexShader, fragmentShader);
        shaders[uuid] = shader;
        return shader;
    }

    Shader *Loader::__loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader, std::string &uuid)
    {
        uuid = uuids::to_string(uuids::uuid_system_generator{}());
        Shader *shader = new Shader();
        shader->loadShader(vertexShader, geometryShader, fragmentShader);
        shaders[uuid] = shader;
        return shader;
    }

    Shader *Loader::loadShader(const std::string &vertexShader, const std::string &fragmentShader)
    {
        std::string uuid;
        return __loadShader(vertexShader, fragmentShader, uuid);
    }

    Shader *Loader::loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string &uuid)
    {
        return __loadShader(vertexShader, fragmentShader, uuid);
    }

    Shader *Loader::loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader)
    {
        std::string uuid;
        return __loadShader(vertexShader, geometryShader, fragmentShader, uuid);
    }

    Shader *Loader::loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader, std::string &uuid)
    {
        return __loadShader(vertexShader, geometryShader, fragmentShader, uuid);
    }

    Shader *Loader::getShader(const std::string &uuid)
    {
        return shaders[uuid];
    }

    SkyBox *Loader::loadSkyBox(const std::string &basePath)
    {
        std::vector<std::string> filenames;
        filenames.push_back(basePath + "/right.tga");
        filenames.push_back(basePath + "/left.tga");
        filenames.push_back(basePath + "/top.tga");
        filenames.push_back(basePath + "/bottom.tga");
        filenames.push_back(basePath + "/back.tga");
        filenames.push_back(basePath + "/front.tga");
        return new SkyBox(filenames);
    }
    
    std::tuple<uint8_t *, int, int> Loader::loadImageData(const std::string &filename)
    {
		int x, y, n;
		int force_channels = 4;
		unsigned char *image_data = stbi_load(filename.c_str(), &x, &y, &n, force_channels);

		if (!image_data)
		{
			fprintf(stderr, "ERROR: could not load %s\n", filename.c_str());
			return std::make_tuple(nullptr, -1, -1);
		}

		if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
			fprintf(
				stderr, "WARNING: texture %s is not power-of-2 dimensions\n", filename.c_str());

		int width_in_bytes = x * 4;
		unsigned char *top = NULL;
		unsigned char *bottom = NULL;
		unsigned char temp = 0;
		int half_height = y / 2;

		for (int row = 0; row < half_height; row++)
		{
			top = image_data + row * width_in_bytes;
			bottom = image_data + (y - row - 1) * width_in_bytes;
			for (int col = 0; col < width_in_bytes; col++)
			{
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				top++;
				bottom++;
			}
		}
        return std::make_tuple(image_data, x, y); 
    }

    ParticleTexture *Loader::loadParticleTexture(const std::vector<std::string> &filenames)
    {
        std::vector<std::tuple<uint8_t*, int, int>> imageData;
        for (auto &filename : filenames)
            imageData.push_back(loadImageData(filename));
        return new ParticleTexture(imageData);  
    }
}