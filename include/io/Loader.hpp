#pragma once

#include <objects/Entity.hpp>
#include <objects/SkyBox.hpp>
#include <graphics/Shader.hpp>
#include <particles/ParticleTexture.hpp>
#include <unordered_map>
#include <tuple>

namespace gps
{

    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &pair) const
        {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };
    class Loader
    {
    private:
        std::unordered_map<std::string, Model3D *> models;
        std::unordered_map<std::string, Entity *> entities;
        std::unordered_map<std::string, Shader *> shaders;
        std::vector<SkyBox *> skyBoxes;
        std::vector<ParticleTexture *> particleTextures;
        
        Entity *__loadEntity(const std::string &filename, std::string &uuid);
        Shader *__loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string &uuid);
        Shader *__loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader, std::string &uuid);
        std::tuple<uint8_t *, int, int> loadImageData(const std::string &filename); 

    public:
        Loader();
        ~Loader();

        Entity* loadEntity(const std::string &filename);
        Entity* loadEntity(const std::string &filename, std::string &uuid);
        Entity* getEntity(const std::string &uuid);
        Shader* loadShader(const std::string &vertexShader, const std::string &fragmentShader);
        Shader* loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string &uuid);
        Shader* loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader);
        Shader* loadShader(const std::string &vertexShader, const std::string &geometryShader, const std::string &fragmentShader, std::string &uuid);
        Shader* getShader(const std::string &uuid);
        SkyBox* loadSkyBox(const std::string &basePath);
        ParticleTexture* loadParticleTexture(const std::vector<std::string> &filenames, bool additive);
    };
}