#include <graphics/ShadowMap.hpp>

namespace gps
{

    ShadowMap::ShadowMap(bool isCubeMap, int shadowWidth, int shadowHeight) : shadowWidth(shadowWidth), shadowHeight(shadowHeight)
    {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &texture);
        if (!isCubeMap)
        {
            glBindTexture(GL_TEXTURE_2D, texture);
            glEnable(GL_DEPTH_TEST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        }
        else
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
            for (int i = 0; i < 6; i++)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                             shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
        }
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ShadowMap::~ShadowMap()
    {
        glDeleteTextures(1, &texture);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);
    }

    void ShadowMap::bindFrameBufferObject()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void ShadowMap::unbindFrameBufferObject()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint ShadowMap::getTextureID()
    {
        return texture;
    }
}