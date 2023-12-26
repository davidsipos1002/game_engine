#include <particles/ParticleRenderer.hpp>
#include <cmath>

namespace gps
{
    ParticleRenderer::ParticleRenderer(ParticleManager *manager, Loader *loader) : manager(manager)
    {
        particleShader = loader->loadShader("shaders/particles.vert", "shaders/particles.frag");
    }
    
    ParticleRenderer::~ParticleRenderer()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vertexVboID);
        glDeleteBuffers(1, &instanceVboID);
        glDeleteVertexArrays(1, &vaoID);
    }

    void ParticleRenderer::init()
    {
        static const float vertices[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
        glGenVertexArrays(1, &vaoID);
        glGenBuffers(1, &vertexVboID);
        glGenBuffers(1, &instanceVboID);

        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVboID);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), 0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (GLvoid *) (4 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (GLvoid *) (8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribDivisor(4, 1);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (GLvoid *) (12 * sizeof(float)));
        glEnableVertexAttribArray(5);
        glVertexAttribDivisor(5, 1);
        glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(ParticleInstanceData), (GLvoid *) (16 * sizeof(float)));
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(6, 1);
        glVertexAttribPointer(6, 1, GL_INT, GL_FALSE, sizeof(ParticleInstanceData), (GLvoid *) (17 * sizeof(float)));
        glEnableVertexAttribArray(7);
        glVertexAttribDivisor(7, 1);
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleInstanceData), (GLvoid *) (18 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ParticleRenderer::render(Camera *camera)
    {
        particleShader->useShaderProgram();
        particleShader->loadMatrix("projectionMatrix", camera->getProjectionMatrix());
        particleShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        particleShader->loadValue("fogDensity", fogDensity);
        particleShader->loadVector("fogColor", fogColor);
        glBindVertexArray(vaoID);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glEnableVertexAttribArray(7);
        glEnable(GL_BLEND);
        glDepthMask(false);

        for (auto &particlePair : manager->particles)
        {
            loadParticleTexture(particlePair.first); 
            ParticleInstanceData *data = getInstanceVboContents(particlePair.first, particlePair.second, camera); 
            updateInstanceVboContents(data, particlePair.second.size());
            delete[] data;
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlePair.second.size());
            unloadParticleTexture(particlePair.first);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);
        glDisableVertexAttribArray(7);
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
        if (texture->isAdditive())
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        else
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    void ParticleRenderer::unloadParticleTexture(ParticleTexture *texture)
    {
        for (int i = 0;i < texture->getCount(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void ParticleRenderer::updateModelMatrix(const Particle &particle, Camera *camera, glm::mat4 &modelMatrix)
    {
        modelMatrix = glm::mat4(1.0f);
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
    }
    
    void ParticleRenderer::getTextureData(ParticleTexture *texture, const Particle &particle, int &i, int &j, float &blendFactor)
    {
        float life = particle.elapsedTime / particle.lifeLength;
        float progress = texture->getCount() * life;
        i = std::floor(progress);
        j = i < texture->getCount() - 1 ? i + 1 : i;
        blendFactor = progress - i;
    }

    ParticleRenderer::ParticleInstanceData *ParticleRenderer::getInstanceVboContents(ParticleTexture *texture, const std::vector<Particle> &particles, Camera *camera)
    {
        ParticleInstanceData *data = new ParticleInstanceData[particles.size()];
        glm::mat4 modelMatrix;
        for (int i = 0; i < particles.size(); i++)
        {
            const Particle &particle = particles[i];
            updateModelMatrix(particle, camera, modelMatrix);
            memcpy(data[i].col0, glm::value_ptr(modelMatrix[0]), 4 * sizeof(float));
            memcpy(data[i].col1, glm::value_ptr(modelMatrix[1]), 4 * sizeof(float));
            memcpy(data[i].col2, glm::value_ptr(modelMatrix[2]), 4 * sizeof(float));
            memcpy(data[i].col3, glm::value_ptr(modelMatrix[3]), 4 * sizeof(float));
            getTextureData(texture, particle, data[i].i, data[i].j, data[i].blendFactor);
        }
        return data;
    }
    
    void ParticleRenderer::updateInstanceVboContents(ParticleInstanceData *data, int size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, instanceVboID);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(ParticleInstanceData), data, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

