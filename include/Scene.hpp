#pragma once

#include <engine/Application.hpp>

using namespace gps;

class Scene : public Application
{
   private:
       
       ParticleEmitter *emitter;
       ParticleEmitter *emitter1;

        void init() override;
        void update(double delta) override;
        bool doCustomRender() override;
        void render() override;
        void cleanup() override;
        void generateGround(int x, int y);
        void generateTreesAndMushrooms(int x, int y);
        
    public:
        Scene(bool fullScreen, int width, int height, const std::string &title);
};