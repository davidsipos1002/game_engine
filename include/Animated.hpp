#pragma once

#include <Animation.hpp>

namespace gps
{
    class Animation;

    class Animated
    {
        friend class Animation;

    private:
        Animation *animation;

    public:
        Animated();
        virtual ~Animated();

        Animation *getAnimation();
    };
}