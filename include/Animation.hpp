#pragma once

#include <GeneralIncludes.hpp>
#include <Animated.hpp>

#include <set>

namespace gps
{

    class Animated;

    class Animation
    {
    public:
        struct KeyFrame
        {
            float offset;
            glm::vec3 translate;
            glm::vec3 rotate;
            glm::vec3 scale;

            KeyFrame(float offset, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) : offset(offset), translate(translate), rotate(rotate), scale(scale) {}
        };
        float speed = 1.0f;

    private:
        struct KeyFrameComparator
        {
            bool operator()(const KeyFrame &a, const KeyFrame &b) const
            {
                return a.offset < b.offset;
            };
        };

        std::set<KeyFrame, KeyFrameComparator> keyframes;
        std::set<KeyFrame, KeyFrameComparator>::iterator previous;
        std::set<KeyFrame, KeyFrameComparator>::iterator current;
        float elapsed = 0;
        bool running = false;

    public:
        glm::vec3 interpolatedTranslation;
        glm::vec3 interpolatedRotation;
        glm::vec3 interpolatedScale;

        Animation();

        void start();
        void stop();
        bool isRunning();
        void addKeyFrame(KeyFrame keyframe);
        void update(double delta);
        void attach(Animated *animated);
    };
}