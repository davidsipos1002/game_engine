#pragma once

#include <Animation.hpp>

#include <vector>
#include <functional>

namespace gps
{
    class Animator
    {
    private:
        float elapsed = 0;
        std::vector<std::pair<std::function<bool()>, AnimationBase *>> triggeredAnimations;
        std::vector<std::tuple<float, float, AnimationBase *>> periodicAnimations;

    public:
        Animator();
        ~Animator();

        template <class T>
        Animation<T> *createTriggeredAnimation(std::function<bool()> trigger)
        {
            Animation<T> *animation = new Animation<T>();
            triggeredAnimations.push_back(std::make_pair(trigger, animation));
            return animation;
        }

        template <class T>
        Animation<T> *createPeriodicAnimation(float start, float period)
        {
            Animation<T> *animation = new Animation<T>();
            periodicAnimations.push_back(std::tuple<float, float, AnimationBase *>(start, period, animation));
            return animation;
        }

        void updateAnimations(double delta);
    };
}