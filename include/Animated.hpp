#pragma once

#include <Animation.hpp>

#include <string>
#include <unordered_map>

namespace gps
{
    template <class T>
    class Animated
    {
    };

    class Entity;
    template <>
    class Animated<Entity>
    {
    private:
        Animation<Entity> *mainComponentAnimation = nullptr;
        std::unordered_map<std::string, Animation<Entity> *> subComponentAnimations;

    public:
        Animation<Entity> *getMainComponentAnimation()
        {
            return mainComponentAnimation;
        }

        Animation<Entity> *getSubComponentAnimation(const std::string &subComponentName)
        {
            if (subComponentAnimations.find(subComponentName) != subComponentAnimations.end())
                return subComponentAnimations.at(subComponentName);
            return nullptr;
        }

        void attachMainComponentAnimation(Animation<Entity> *animation)
        {
            mainComponentAnimation = animation;
        }

        void attachSubComponentAnimation(Animation<Entity> *animation, const std::string &subComponentName)
        {
            subComponentAnimations[subComponentName] = animation;
        }
    };
}