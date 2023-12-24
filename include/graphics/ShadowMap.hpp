#pragma once

#include <engine/GeneralIncludes.hpp>

namespace gps
{
    class ShadowMap
    {
    private:
        GLuint fbo;
        GLuint texture;

    public:
        int shadowWidth;
        int shadowHeight;

        ShadowMap(bool isCubeMap, int shadowWidth, int shadowHeight);
        ~ShadowMap();

        void bindFrameBufferObject();
        void unbindFrameBufferObject();
        GLuint getTextureID();
    };
}