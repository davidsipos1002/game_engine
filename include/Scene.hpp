#pragma once

#include <engine/Application.hpp>

using namespace gps;

class Scene : public Application
{
   private:
        std::string teapot1;
        std::string teapot2;
        std::string shader;
        std::string dirShadowMap;
        std::string quad;
        std::string quadEntity;

        ParticleEmitter *emitter;

        void init() override;
        void update(double delta) override;
        bool doCustomRender() override;
        void render() override;
        void cleanup() override;
        
    public:
        Scene(bool fullScreen, int width, int height, const std::string &title);
};