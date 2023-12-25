#pragma once

#include <animation/Animation.hpp>

#include <string>
#include <unordered_map>

namespace gps
{
    template <class T>
    class Animated
    {
        private:
            Animation<T> *animation = nullptr;
            
        public:
            Animated() {};
            virtual ~Animated() {};

            Animation<T> *getAnimation() const
            {
                return animation;
            }

            void setAnimation(Animation<T> *animation)
            {
                this->animation = animation;
            }
    };

    class Entity;
    template <>
    class Animated<Entity>
    {
    private:
        Animation<Entity> *mainComponentAnimation = nullptr;
        std::unordered_map<std::string, Animation<Entity> *> subComponentAnimations;

    public:
        virtual ~Animated() {}

        Animation<Entity> *getMainComponentAnimation() const
        {
            return mainComponentAnimation;
        }

        Animation<Entity> *getSubComponentAnimation(const std::string &subComponentName) const
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