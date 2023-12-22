#pragma once

#include <engine/GeneralIncludes.hpp>

#include <iostream>
#include <set>

namespace gps
{
    template <class T>
    class KeyFrame
    {
    };

    class Entity;
    class DirectionalLight;
    class PointLight;
    class SpotLight;

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
    
    template <>
    class KeyFrame<DirectionalLight>
    {
        public:
            float offset;
            glm::vec3 direction;
            glm::vec3 color;
            float intensity;
            
            KeyFrame() : offset(0), direction(glm::vec3(0, 0, 0)), color(glm::vec3(0, 0, 0)), intensity(0) {}
            KeyFrame(float offset, glm::vec3 direction, glm::vec3 color, float intensity) : offset(offset), direction(direction), color(color), intensity(intensity) {}

            KeyFrame operator*(float other) const
            {
                KeyFrame ret = *this;
                ret.direction *= other;
                ret.color *= other;
                ret.intensity *= other;
                return ret;
            }
            
            KeyFrame operator+(const KeyFrame &other) const
            {
                KeyFrame ret = *this;
                ret.direction += other.direction;
                ret.color += other.color;
                ret.intensity += other.intensity;
                return ret;
            }
    };

    template <>
    class KeyFrame<PointLight>
    {
        public:
            float offset;
            glm::vec3 position;
            glm::vec3 color;
            float intensity;
            
            KeyFrame() : offset(0), position(glm::vec3(0, 0, 0)), color(glm::vec3(0, 0, 0)), intensity(0) {}
            KeyFrame(float offset, glm::vec3 position, glm::vec3 color, float intensity) : offset(offset), position(position), color(color), intensity(intensity) {}

            KeyFrame operator*(float other) const
            {
                KeyFrame ret = *this;
                ret.position *= other;
                ret.color *= other;
                ret.intensity *= other;
                return ret;
            }
            
            KeyFrame operator+(const KeyFrame &other) const
            {
                KeyFrame ret = *this;
                ret.position += other.position;
                ret.color += other.color;
                ret.intensity += other.intensity;
                return ret;
            }
    };
    
    template <>
    class KeyFrame<SpotLight>
    {
        public:
            float offset;
            glm::vec3 position;
            glm::vec3 direction;
            glm::vec3 color;
            float cutoff;
            float intensity;
            
            KeyFrame() : offset(0), position(glm::vec3(0, 0, 0)), direction(glm::vec3(0, 0, 0)), color(glm::vec3(0, 0, 0)), cutoff(0), intensity(0) {}
            KeyFrame(float offset, glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutoff, float intensity) : offset(offset), position(position), direction(direction), color(color), cutoff(cutoff), intensity(intensity) {}

            KeyFrame operator*(float other) const
            {
                KeyFrame ret = *this;
                ret.position *= other;
                ret.direction *= other;
                ret.color *= other;
                ret.cutoff *= other;
                ret.intensity *= other;
                return ret;
            }
            
            KeyFrame operator+(const KeyFrame &other) const
            {
                KeyFrame ret = *this;
                ret.position += other.position;
                ret.direction += other.direction;
                ret.color += other.color;
                ret.cutoff += other.cutoff;
                ret.intensity += other.intensity;
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