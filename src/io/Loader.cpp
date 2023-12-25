#include <io/Loader.hpp>

#define UUID_SYSTEM_GENERATOR
#include <io/uuid.h>

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
        uuid = uuids::to_string (uuids::uuid_system_generator{}());
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
    
    Entity* Loader::getEntity(const std::string &uuid) 
    {
        return entities[uuid];
    }

    Shader *Loader::__loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string &uuid)
    {
        uuid = uuids::to_string (uuids::uuid_system_generator{}());
        Shader *shader = new Shader();
        shader->loadShader(vertexShader, fragmentShader);
        shaders[uuid] = shader;
        return shader;
    }
    
    Shader *Loader::__loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader, std::string &uuid)
    {
        uuid = uuids::to_string (uuids::uuid_system_generator{}());
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
}