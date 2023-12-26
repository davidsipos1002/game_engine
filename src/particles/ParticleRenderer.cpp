#include <particles/ParticleRenderer.hpp>
#include <cmath>

namespace gps
{
    ParticleRenderer::ParticleRenderer(ParticleManager *manager, Loader *loader) : manager(manager)
    {
        particleShader = loader->loadShader("shaders/particles.vert", "shaders/particles.frag");
    }

    void ParticleRenderer::init()
    {
        static const float vertices[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
        glGenVertexArrays(1, &vaoID);
        glGenBuffers(1, &vboID);
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ParticleRenderer::render(Camera *camera)
    {
        particleShader->useShaderProgram();
        particleShader->loadMatrix("projectionMatrix", camera->getProjectionMatrix());
        particleShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        glBindVertexArray(vaoID);
        glEnableVertexAttribArray(0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(false);

        for (auto &particlePair : manager->particles)
        {
            loadParticleTexture(particlePair.first); 
            for (auto &particle : particlePair.second)
            {
                loadModelMatrix(particle, camera);
    
                if (particle.additive)
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                else
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
                int i, j;
                float blendFactor;
                getTextureData(particlePair.first, particle, i, j, blendFactor);
                particleShader->loadValue("i", i);
                particleShader->loadValue("j", j);
                particleShader->loadValue("blendFactor", blendFactor);

                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
            unloadParticleTexture(particlePair.first);
        }

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(true);
    }
    
    void ParticleRenderer::loadParticleTexture(ParticleTexture *texture)
    {
        for (int i = 0;i < texture->getCount(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            particleShader->loadValue("particleTexture[" + std::to_string(i) + "]", i);
            glBindTexture(GL_TEXTURE_2D, texture->getTexture(i)); 
        }
    }
    
    void ParticleRenderer::unloadParticleTexture(ParticleTexture *texture)
    {
        for (int i = 0;i < texture->getCount(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void ParticleRenderer::loadModelMatrix(const Particle &particle, Camera *camera)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix[0][0] = camera->getViewMatrix()[0][0];
        modelMatrix[0][1] = camera->getViewMatrix()[1][0];
        modelMatrix[0][2] = camera->getViewMatrix()[2][0];
        modelMatrix[1][0] = camera->getViewMatrix()[0][1];
        modelMatrix[1][1] = camera->getViewMatrix()[1][1];
        modelMatrix[1][2] = camera->getViewMatrix()[2][1];
        modelMatrix[2][0] = camera->getViewMatrix()[0][2];
        modelMatrix[2][1] = camera->getViewMatrix()[1][2];
        modelMatrix[2][2] = camera->getViewMatrix()[2][2];
        glm::mat4 translate = glm::translate(glm::mat4(1), particle.position);
        glm::mat4 rotate = glm::rotate(glm::mat4(1), particle.rotation, glm::vec3(0, 0, 1));
        glm::mat scale = glm::scale(glm::mat4(1), glm::vec3(particle.scale, particle.scale, particle.scale));
        modelMatrix = translate * scale * rotate * modelMatrix;
        particleShader->loadMatrix("modelMatrix", modelMatrix);
    }
    
    void ParticleRenderer::getTextureData(ParticleTexture *texture, const Particle &particle, int &i, int &j, float &blendFactor)
    {
        float life = particle.elapsedTime / particle.lifeLength;
        float progress = texture->getCount() * life;
        i = std::floor(progress);
        j = i < texture->getCount() - 1 ? i + 1 : i;
        blendFactor = progress - i;
    }
}
