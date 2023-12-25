#include <graphics/Renderer.hpp>
#include <engine/ErrorCheck.hpp>

namespace gps
{
    Renderer::Renderer(Window *window, Loader *loader) : window(window)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, window->getWindowDimensions().width, window->getWindowDimensions().height);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        for (auto &pair : directionalLights)
            pair.second = new ShadowMap(false, window->getWindowDimensions().width, window->getWindowDimensions().height);
        for (auto &pair: pointLights)
            pair.second = new ShadowMap(true, 1024, 1024);
        for (auto &pair : spotLights)
            pair.second = new ShadowMap(false, window->getWindowDimensions().width, window->getWindowDimensions().height);
        entityShader = loader->loadShader("shaders/entityNormal.vert", "shaders/entityNormal.frag");
        directionalAndSpotShadowShader = loader->loadShader("shaders/directionalAndSpotShadow.vert", "shaders/directionalAndSpotShadow.frag");
        pointShadowShader = loader->loadShader("shaders/pointShadow.vert", "shaders/pointShadow.geom", "shaders/pointShadow.frag");
        shadowMapShader = loader->loadShader("shaders/shadowMap.vert", "shaders/shadowMap.frag");
    }

    Renderer::~Renderer()
    {
        for (auto &pair : directionalLights)
            delete pair.second;
        for (auto &pair : pointLights)
            delete pair.second;
        for (auto &pair : spotLights)
            delete pair.second;
    }

    void Renderer::addEntity(Entity *entity)
    {
        entities[entity->getModel()].push_back(entity);
    }

    void Renderer::renderModels(Model3D *model)
    {
        std::vector<Entity *> &v = entities[model];

        for (auto &meshPair : model->meshes)
        {
            const std::string &name = meshPair.first;
            Mesh *mesh = meshPair.second;
            Buffers buff = mesh->getBuffers();
            std::vector<Texture> &textures = mesh->textures;

            for (GLuint i = 0; i < textures.size(); i++)
            {
                glCheckError();
                glActiveTexture(GL_TEXTURE0 + i);
                glCheckError();
                glUniform1i(glGetUniformLocation(entityShader->shaderProgram, textures[i].type.c_str()), i);
                glCheckError();
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                glCheckError();
            }
            glBindVertexArray(buff.VAO);
            glCheckError();

            for (auto entity : v)
            {
                glm::mat4 scale = glm::scale(glm::mat4(1), entity->scale);
                glm::mat4 rotate = glm::rotate(glm::mat4(1), entity->rotation.x, glm::vec3(1.0f, 0, 0));
                rotate = glm::rotate(rotate, entity->rotation.y, glm::vec3(0, 1.0f, 0));
                rotate = glm::rotate(rotate, entity->rotation.z, glm::vec3(0, 0, 1.0f));
                glm::mat4 translate = glm::translate(glm::mat4(1), entity->position);
                getEntityAnimationMatrices(entity->getMainComponentAnimation(), translate, rotate, scale);
                getEntityAnimationMatrices(entity->getSubComponentAnimation(name), translate, rotate, scale);
                glm::mat4 modelMatrix = translate * rotate * scale;
                entityShader->loadMatrix("modelMatrix", modelMatrix);
                entityShader->loadValue("ambientStrength", entity->ambientStrength);
                entityShader->loadValue("specularStrength", entity->specularStrength);
                glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
            }

            glBindVertexArray(0);
            for (GLuint i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }

    void Renderer::renderEntities(Camera *camera, glm::mat4 projectionMatrix)
    {
        renderShadowMaps();
        entityShader->useShaderProgram();
        loadShadowMaps(entityShader);
        entityShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        entityShader->loadMatrix("projectionMatrix", projectionMatrix);
        glCheckError();
        for (int i = 0; i < directionalLights.size(); i++)
            directionalLights[i].first.loadUniforms(entityShader, i);
        glCheckError();
        for (int i = 0; i < pointLights.size(); i++)
            pointLights[i].first.loadUniforms(entityShader, i);
        glCheckError();
        for (int i = 0; i < spotLights.size(); i++) 
        {
            if (i < 5)
            {
                entityShader->loadValue("spotLightIsShadowCasting[" + std::to_string(i) + "]", spotLights[i].first.isShadowCasting);
                entityShader->loadMatrix("spotLightSpaceMatrix[" + std::to_string(i) + "]", spotLights[i].first.getLightMatrix());
            }
            spotLights[i].first.loadUniforms(entityShader, i);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCheckError();
        for (auto &pair : entities)
            renderModels(pair.first);
        glCheckError();
    }

    void Renderer::renderShadowMapModels(Model3D *model, Shader *shader)
    {
        std::vector<Entity *> &v = entities[model];

        for (auto &meshPair : model->meshes)
        {
            const std::string &name = meshPair.first;
            Mesh *mesh = meshPair.second;
            Buffers buff = mesh->getBuffers();
            glBindVertexArray(buff.VAO);
            for (auto entity : v)
            {
                glm::mat4 scale = glm::scale(glm::mat4(1), entity->scale);
                glm::mat4 rotate = glm::rotate(glm::mat4(1), entity->rotation.x, glm::vec3(1.0f, 0, 0));
                rotate = glm::rotate(rotate, entity->rotation.y, glm::vec3(0, 1.0f, 0));
                rotate = glm::rotate(rotate, entity->rotation.z, glm::vec3(0, 0, 1.0f));
                glm::mat4 translate = glm::translate(glm::mat4(1), entity->position);
                getEntityAnimationMatrices(entity->getMainComponentAnimation(), translate, rotate, scale);
                getEntityAnimationMatrices(entity->getSubComponentAnimation(name), translate, rotate, scale);
                glm::mat4 modelMatrix = translate * rotate * scale;
                shader->loadMatrix("modelMatrix", modelMatrix);
                glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
            }
            glBindVertexArray(0);
        }
    }

    void Renderer::renderDirectionalAndSpotLightShadowMapEntities(Light &light, ShadowMap *map)
    {
        light.calculateLightMatrices(map->shadowWidth, map->shadowHeight);
        directionalAndSpotShadowShader->useShaderProgram();
        directionalAndSpotShadowShader->loadMatrix("lightSpaceMatrix", light.getLightMatrix());
        glViewport(0, 0, map->shadowWidth, map->shadowHeight);
        map->bindFrameBufferObject();
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto &pair : entities)
            renderShadowMapModels(pair.first, directionalAndSpotShadowShader);
        map->unbindFrameBufferObject();
    }
    
    void Renderer::renderPointLightShadowMapEntities(PointLight &light, ShadowMap *map)
    {
        light.calculateLightMatrices(map->shadowWidth, map->shadowHeight);
        pointShadowShader->useShaderProgram();
        pointShadowShader->loadVector("lightPosition", light.lightPosition);
        for (int i = 0; i < 6; i++)
            pointShadowShader->loadMatrix("lightSpaceMatrices[" + std::to_string(i) + "]", light.getLightMatrix(i));
        glViewport(0, 0, map->shadowWidth, map->shadowHeight);
        map->bindFrameBufferObject();
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto &pair : entities)
            renderShadowMapModels(pair.first, pointShadowShader);
        map->unbindFrameBufferObject();
    }

    void Renderer::getEntityAnimationMatrices(Animation<Entity> *animation, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale)
    {
        if (animation)
        {
            translate = glm::translate(translate, animation->interpolatedKeyFrame.translate);
            rotate = glm::rotate(rotate, animation->interpolatedKeyFrame.rotate.x, glm::vec3(1, 0, 0));
            rotate = glm::rotate(rotate, animation->interpolatedKeyFrame.rotate.y, glm::vec3(0, 1, 0));
            rotate = glm::rotate(rotate, animation->interpolatedKeyFrame.rotate.z, glm::vec3(0, 0, 1));
            scale = glm::scale(scale, animation->interpolatedKeyFrame.scale);
        }
    }

    void Renderer::__renderShadowMap(Entity *quad, ShadowMap *shadowMap)
    {
        glViewport(0, 0, window->getWindowDimensions().width, window->getWindowDimensions().height);
        glClear(GL_COLOR_BUFFER_BIT);
        shadowMapShader->useShaderProgram();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap->getTextureID());
        shadowMapShader->loadValue("shadowMap", 0);
        glDisable(GL_DEPTH_TEST);
        quad->getModel()->Draw(shadowMapShader);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::displayDirectionalAndSpotLightShadowMap(Entity *quad, int i)
    {
        renderDirectionalAndSpotLightShadowMapEntities(spotLights[i].first, spotLights[i].second);
        __renderShadowMap(quad, spotLights[i].second);
    }

    void Renderer::renderShadowMaps()
    {
        for (auto &pair : directionalLights)
            if (pair.first.isShadowCasting)
                renderDirectionalAndSpotLightShadowMapEntities(pair.first, pair.second);
        for (int i = 0;i < spotLights.size() / 2; i++)
            if (spotLights[i].first.isShadowCasting)
                renderDirectionalAndSpotLightShadowMapEntities(spotLights[i].first, spotLights[i].second);
    }

    void Renderer::loadShadowMaps(Shader *shader)
    {
        GLuint val = 31;
        for (int i = 0; i < directionalLights.size(); i++)
        {
            if (directionalLights[i].first.isShadowCasting)
            {
                glActiveTexture(GL_TEXTURE0 + val);
                glBindTexture(GL_TEXTURE_2D, directionalLights[i].second->getTextureID());
                shader->loadValue("directionalLightShadowMap[" + std::to_string(i) + "]", val);
                val--;
            }
        }
        for (int i = 0; i < spotLights.size() / 2; i++)
        {
            if (spotLights[i].first.isShadowCasting)
            {
                glActiveTexture(GL_TEXTURE0 + val);
                glBindTexture(GL_TEXTURE_2D, spotLights[i].second->getTextureID());
                shader->loadValue("spotLightShadowMap[" + std::to_string(i) + "]", val);
                val--;
            }
        }
    }

    DirectionalLight &Renderer::getDirectionalLight(int index)
    {
        return directionalLights[index].first;
    }

    PointLight &Renderer::gePointLight(int index)
    {
        return pointLights[index].first;
    }

    SpotLight &Renderer::getSpotLight(int index)
    {
        return spotLights[index].first;
    }
}