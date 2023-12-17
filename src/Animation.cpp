#include <Animation.hpp>

namespace gps
{

    Animation::Animation()
    {
        keyframes.insert(KeyFrame(0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));
    }

    void Animation::start()
    {
        previous = keyframes.begin();
        current = std::next(previous);
        elapsed = 0;
        if (current == keyframes.end())
            return;
        interpolatedTranslation = glm::vec3(0, 0, 0);
        interpolatedRotation = glm::vec3(0, 0, 0);
        interpolatedScale = glm::vec3(0, 0, 0);
        running = true;
    }

    void Animation::stop()
    {
        running = false;
    }

    bool Animation::isRunning()
    {
        return running;
    }

    void Animation::addKeyFrame(KeyFrame keyframe)
    {
        keyframes.insert(keyframe);
    }

    void Animation::update(double delta)
    {
        float advance = delta * speed;
        elapsed += advance;
        if (elapsed >= current->offset)
        {
            previous = current;
            current++;
            if (current == keyframes.end())
            {
                running = false;
                return;
            }
        }
        float currentPos = elapsed - previous->offset;
        float t = currentPos / (current->offset - previous->offset);
        interpolatedTranslation = (1.0f - t) * previous->translate + t * current->translate;
        interpolatedRotation = (1.0f - t) * previous->rotate + t * current->rotate;
        interpolatedScale = (1.0f - t) * previous->scale + t * current->scale;
    }

    void Animation::attach(Animated *animated)
    {
        animated->animation = this;
    }
}