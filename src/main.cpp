#include <Scene.hpp>

int main(int argc, const char *argv[])
{
    Scene scene(true, 1024, 768, "OpenGL Project");
    scene.run();
    return EXIT_SUCCESS;
}
