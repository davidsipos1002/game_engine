#include <Animated.hpp>

namespace gps
{
    Animated::Animated() : animation(nullptr) {}

    Animated::~Animated() {}

    Animation *Animated::getAnimation()
    {
        return animation;
    }
}
