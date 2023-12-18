#include <Animator.hpp>

#include <limits>

namespace gps
{

    Animator::Animator() {}

    Animator::~Animator()
    {
        for (auto &pair : triggeredAnimations)
            delete pair.second;
        for (auto &tuple : periodicAnimations)
            delete std::get<2>(tuple);
    }

    void Animator::updateAnimations(double delta)
    {
        elapsed += delta;
        for (auto &pair : triggeredAnimations)
        {
            std::function<bool()> &trigger = pair.first;
            AnimationBase *animation = pair.second;

            if (!animation->isRunning())
            {
                if (trigger())
                    animation->start();
            }
            else
                animation->update(delta);
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