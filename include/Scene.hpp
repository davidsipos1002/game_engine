#pragma once

#include <engine/Application.hpp>

using namespace gps;

class Scene : public Application
{
private:
    std::vector<ParticleEmitter *> emitters;
    PointLight *campfire;
    Entity *quad;

    void init() override;
    void update(double delta) override;
    bool doCustomRender() override;
    void render() override;
    void cleanup() override;
    void generateGround(int x, int y);
    void generateTrees(int x, int y, const glm::vec3 &center);
    void setupCampingPlace(const glm::vec3 &position);
    void setupForestCabin(const glm::vec3 &position);
    void setupCameraAnimation();

public:
    Scene(bool fullScreen, int width, int height, const std::string &title);
};