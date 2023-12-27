#include <Scene.hpp>

Scene::Scene(bool fullScreen, int width, int height, const std::string &title) : Application(fullScreen, width, height, title)
{
}

void Scene::init()
{
    skyBox = loader->loadSkyBox("nightsky");
}

void Scene::update(double delta)
{
}

bool Scene::doCustomRender()
{
    return false;
}

void Scene::render()
{
}

void Scene::cleanup()
{
}