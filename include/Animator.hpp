#pragma once

#include <Animation.hpp>

#include <vector>
#include <functional>

namespace gps
{
    class Animator
    {
    private:
        std::vector<std::pair<std::function<bool()>, Animation *>> animations;

    public:
        Animator();
        ~Animator();

        Animation *createTriggeredAnimation(std::function<bool()> trigger);
        void updateAnimations(double delta);
    };
}