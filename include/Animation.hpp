#pragma once

#include <GeneralIncludes.hpp>

#include <iostream>
#include <set>

namespace gps
{
    template <class T>
    class KeyFrame
    {
    };

    class Entity;

    template <>
    class KeyFrame<Entity>
    {
    public:
        float offset;
        glm::vec3 translate;
        glm::vec3 rotate;
        glm::vec3 scale;

        KeyFrame() : offset(0), translate(glm::vec3(0, 0, 0)), rotate(glm::vec3(0, 0, 0)), scale(glm::vec3(1, 1, 1)) {}
        KeyFrame(float offset, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) : offset(offset), translate(translate), rotate(rotate), scale(scale) {}

        KeyFrame operator*(float other) const
        {
            KeyFrame ret = *this;
            ret.translate *= other;
            ret.rotate *= other;
            ret.scale *= other;
            return ret;
        }

        KeyFrame operator+(const KeyFrame &other) const
        {
            KeyFrame ret = *this;
            ret.translate += other.translate;
            ret.rotate += other.rotate;
            ret.scale += other.scale;
            return ret;
        }
    };

    template <class T>
    struct KeyFrameComparator
    {
        bool operator()(const KeyFrame<T> &a, const KeyFrame<T> &b) const
        {
            return a.offset < b.offset;
        };
    };

    class AnimationBase
    {
    public:
        float duration = 0;

        virtual ~AnimationBase() {}
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual bool isRunning() = 0;
        virtual void update(double delta) = 0;
    };

    template <class T>
    class Animation : public AnimationBase
    {
    private:
        std::set<KeyFrame<T>, KeyFrameComparator<T>> keyframes;
        std::set<KeyFrame<T>, KeyFrameComparator<T>>::iterator previous;
        std::set<KeyFrame<T>, KeyFrameComparator<T>>::iterator current;
        float elapsed = 0;
        bool running;

    public:
        KeyFrame<T> interpolatedKeyFrame;

        Animation() : running(false)
        {
            addKeyFrame(KeyFrame<T>());
        };

        void start() override
        {
            previous = keyframes.begin();
            current = std::next(previous);
            elapsed = 0;
            if (current == keyframes.end())
                return;
            running = true;
        }

        void stop() override
        {
            running = false;
        }

        bool isRunning() override
        {
            return running;
        }

        void addKeyFrame(const KeyFrame<T> &keyframe)
        {
            duration = std::max(duration, keyframe.offset);
            keyframes.insert(keyframe);
        }

        void update(double delta) override
        {
            elapsed += delta;
            if (std::isgreaterequal(elapsed, current->offset))
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
            interpolatedKeyFrame = (*previous) * (1.0f - t) + (*current) * t;
        }
    };
}