#include <animation/Animator.hpp>

#include <limits>

namespace gps
{

    Animator::Animator() {}

    Animator::~Animator()
    {
        for (auto &tuple : triggeredAnimations)
            delete std::get<2>(tuple);
        for (auto &tuple : periodicAnimations)
            delete std::get<2>(tuple);
    }

    void Animator::updateAnimations(double delta)
    {
        elapsed += delta;
        for (std::vector<std::tuple<std::function<bool()>, bool, AnimationBase *>>::iterator it = triggeredAnimations.begin(); it != triggeredAnimations.end();)
        {
            std::function<bool()> &trigger = std::get<0>(*it);
            bool oneTime = std::get<1>(*it);
            AnimationBase *animation = std::get<2>(*it);

            if (animation->hasEnded()) 
            {
                it = triggeredAnimations.erase(it);
                continue;
            }

            if (!animation->isRunning())
            {
                if (trigger())
                    animation->start();
            }
            else
                animation->update(delta);
            it++;
        }
        for (auto &tuple : periodicAnimations)
        {
            float &start = std::get<0>(tuple);
            float period = std::get<1>(tuple);
            AnimationBase *animation = std::get<2>(tuple);
            
            if (!animation->isRunning())
            {
                if (std::isgreaterequal(elapsed, start))
                {
                    animation->start();
                    if (std::isgreater(period, 0))
                        start += animation->duration + period;
                    else
                        start = std::numeric_limits<float>::infinity();
                }
            }   
            else
                animation->update(delta);
        }
    }
}