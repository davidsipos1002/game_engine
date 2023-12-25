#pragma once

#include <engine/GeneralIncludes.hpp>

namespace gps
{
    class ShadowMap
    {
    private:
        GLuint fbo;
        GLuint texture;
        int shadowWidth;
        int shadowHeight;

    public:

        ShadowMap(bool isCubeMap, int shadowWidth, int shadowHeight);
        ~ShadowMap();

        void bindFrameBufferObject();
        void unbindFrameBufferObject();
        GLuint getTextureID();
        int getShadowWidth();
        int getShadowHeight();
    };
}