#include <Animator.hpp>

namespace gps
{

    Animator::Animator() {}

    Animator::~Animator()
    {
        for (auto pair : animations)
            delete pair.second;
    }

    Animation *Animator::createTriggeredAnimation(std::function<bool()> trigger)
    {
        Animation *animation = new Animation();
        animations.push_back(std::make_pair(trigger, animation));
        return animation;
    }

    void Animator::updateAnimations(double delta)
    {
        for (auto &pair : animations)
        {
            std::function<bool()> &trigger = pair.first;
            Animation *animation = pair.second;

            if (!animation->isRunning())
            {
                if (trigger())
                    animation->start();
            }
            else
                animation->update(delta);
        }
    }
}