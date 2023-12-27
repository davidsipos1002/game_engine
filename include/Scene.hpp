#pragma once

#include <engine/Application.hpp>

using namespace gps;

class Scene : public Application
{
   private:
        void init() override;
        void update(double delta) override;
        bool doCustomRender() override;
        void render() override;
        void cleanup() override;
        
    public:
        Scene(bool fullScreen, int width, int height, const std::string &title);
};