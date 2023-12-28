#include <graphics/Renderer.hpp>
#include <engine/ErrorCheck.hpp>

namespace gps
{
    Renderer::Renderer(Window *window, Loader *loader, SkyBox **skyBox) : window(window), skyBox(skyBox)
    {
        initOpenGL();
        initLights();
        loadShaders(loader);
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

    void Renderer::initOpenGL()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, window->getWindowDimensions().width, window->getWindowDimensions().height);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void Renderer::initLights()
    {
        for (auto &pair : directionalLights)
            pair.second = new ShadowMap(false, window->getWindowDimensions().width / 2, window->getWindowDimensions().height / 2);
        for (auto &pair : pointLights)
            pair.second = new ShadowMap(true, 2048, 2048);
        for (auto &pair : spotLights)
            pair.second = new ShadowMap(false, window->getWindowDimensions().width / 2, window->getWindowDimensions().height / 2);
    }

    void Renderer::loadShaders(Loader *loader)
    {
        entityShader = loader->loadShader("shaders/entity/entityNormal.vert", "shaders/entity/entityNormal.frag");
        instancedEntityShader = loader->loadShader("shaders/entity/entityInstanced.vert", "shaders/entity/entityNormal.frag");

        directionalAndSpotShadowShader = loader->loadShader("shaders/directionalAndSpot/directionalAndSpotShadow.vert", "shaders/directionalAndSpot/directionalAndSpotShadow.frag");
        instancedDirectionalAndSpotShadowShader = loader->loadShader("shaders/directionalAndSpot/directionalAndSpotShadowInstanced.vert", "shaders/directionalAndSpot/directionalAndSpotShadow.frag");

        pointShadowShader = loader->loadShader("shaders/point/pointShadow.vert", "shaders/point/pointShadow.geom", "shaders/point/pointShadow.frag");
        instancedPointShadowShader = loader->loadShader("shaders/point/pointShadowInstanced.vert", "shaders/point/pointShadow.geom", "shaders/point/pointShadow.frag");

        shadowMapShader = loader->loadShader("shaders/shadow/shadowMap.vert", "shaders/shadow/shadowMap.frag");

        facesShader = loader->loadShader("shaders/faces/faces.vert", "shaders/faces/faces.geom", "shaders/faces/faces.frag");
        instancedFacesShader = loader->loadShader("shaders/faces/facesInstanced.vert", "shaders/faces/faces.geom", "shaders/faces/faces.frag");

        skyboxShader = loader->loadShader("shaders/skybox/skybox.vert", "shaders/skybox/skybox.frag");
    }

    void Renderer::loadLightsToShader(Shader *shader)
    {
        for (int i = 0; i < directionalLights.size(); i++)
            directionalLights[i].first.loadUniforms(shader, i);
        for (int i = 0; i < pointLights.size(); i++)
        {
            if (i < 5)
                shader->loadValue("pointLightIsShadowCasting[" + std::to_string(i) + "]", pointLights[i].first.isShadowCasting);
            pointLights[i].first.loadUniforms(shader, i);
        }
        for (int i = 0; i < spotLights.size(); i++)
        {
            if (i < 5)
            {
                shader->loadValue("spotLightIsShadowCasting[" + std::to_string(i) + "]", spotLights[i].first.isShadowCasting);
                shader->loadMatrix("spotLightSpaceMatrix[" + std::to_string(i) + "]", spotLights[i].first.getLightMatrix());
            }
            spotLights[i].first.loadUniforms(shader, i);
        }
    }

    void Renderer::loadShadowMaps(Shader *shader)
    {
        GLuint val = 30;
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
        for (int i = 0; i < pointLights.size() / 2; i++)
        {
            if (pointLights[i].first.isShadowCasting)
            {
                glActiveTexture(GL_TEXTURE0 + val);
                glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[i].second->getTextureID());
                shader->loadValue("pointLightShadowMap[" + std::to_string(i) + "]", val);
                val--;
            }
            else
                shader->loadValue("pointLightShadowMap[" + std::to_string(i) + "]", 31);
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

    void Renderer::computeEntityModelMatrix(Entity *entity, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale)
    {
        scale = glm::scale(glm::mat4(1), entity->scale);
        rotate = glm::rotate(glm::mat4(1), entity->rotation.x, glm::vec3(1.0f, 0, 0));
        rotate = glm::rotate(rotate, entity->rotation.y, glm::vec3(0, 1.0f, 0));
        rotate = glm::rotate(rotate, entity->rotation.z, glm::vec3(0, 0, 1.0f));
        translate = glm::translate(glm::mat4(1), entity->position);
        getEntityAnimationMatrices(entity->getMainComponentAnimation(), translate, rotate, scale);
    }

    void Renderer::addInstancedEntity(Entity *entity)
    {
        if (instancedEntities.find(entity->getModel()) == instancedEntities.end())
        {
            ModelMetadata metadata;
            glGenBuffers(1, &metadata.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, metadata.vbo);
            for (auto &mesh : entity->getModel()->meshes)
            {
                glBindVertexArray(mesh.second->getBuffers().VAO);

                glEnableVertexAttribArray(3);
                glVertexAttribDivisor(3, 1);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(EntityInstanceData),
                                      (GLvoid *)offsetof(EntityInstanceData, modelMatrix.col0));

                glEnableVertexAttribArray(4);
                glVertexAttribDivisor(4, 1);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(EntityInstanceData),
                                      (GLvoid *)offsetof(EntityInstanceData, modelMatrix.col1));

                glEnableVertexAttribArray(5);
                glVertexAttribDivisor(5, 1);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(EntityInstanceData),
                                      (GLvoid *)offsetof(EntityInstanceData, modelMatrix.col2));

                glEnableVertexAttribArray(6);
                glVertexAttribDivisor(6, 1);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(EntityInstanceData),
                                      (GLvoid *)offsetof(EntityInstanceData, modelMatrix.col3));

                glEnableVertexAttribArray(7);
                glVertexAttribDivisor(7, 1);
                glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(EntityInstanceData),
                                      (GLvoid *)offsetof(EntityInstanceData, lightData));

                glBindVertexArray(0);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            metadata.bufferSize = 10;
            metadata.instanceData = new EntityInstanceData[10];
            metadata.entities.push_back(entity);
            instancedEntities[entity->getModel()] = metadata;
        }
        else
        {
            ModelMetadata &metadata = instancedEntities[entity->getModel()];
            metadata.entities.push_back(entity);
            if (metadata.entities.size() > metadata.bufferSize)
            {
                metadata.bufferSize += 10;
                delete[] metadata.instanceData;
                metadata.instanceData = new EntityInstanceData[metadata.bufferSize];
            }
        }
    }

    void Renderer::prepareInstances()
    {
        glm::mat4 translate, rotate, scale, modelMatrix;
        for (auto &modelPair : instancedEntities)
        {
            const ModelMetadata &metadata = modelPair.second;
            EntityInstanceData *data = metadata.instanceData;
            const std::vector<Entity *> &v = metadata.entities;
            for (int i = 0; i < v.size(); i++)
            {
                computeEntityModelMatrix(v[i], translate, rotate, scale);
                modelMatrix = translate * rotate * scale;
                memcpy(data[i].modelMatrix.col0, glm::value_ptr(modelMatrix[0]), 4 * sizeof(float));
                memcpy(data[i].modelMatrix.col1, glm::value_ptr(modelMatrix[1]), 4 * sizeof(float));
                memcpy(data[i].modelMatrix.col2, glm::value_ptr(modelMatrix[2]), 4 * sizeof(float));
                memcpy(data[i].modelMatrix.col3, glm::value_ptr(modelMatrix[3]), 4 * sizeof(float));
                data[i].lightData[0] = v[i]->ambientStrength;
                data[i].lightData[1] = v[i]->specularStrength;
            }
            glBindBuffer(GL_ARRAY_BUFFER, metadata.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(EntityInstanceData) * v.size(), data, GL_STREAM_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void Renderer::renderInstancedModels(Model3D *model, ModelMetadata &metadata, bool isShadowMapRender)
    {
        for (auto &meshPair : model->meshes)
        {
            Mesh *mesh = meshPair.second;
            Buffers buff = mesh->getBuffers();
            std::vector<Texture> &textures = mesh->textures;

            if (!isShadowMapRender)
            {
                for (GLuint i = 0; i < textures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    instancedEntityShader->loadValue(textures[i].type.c_str(), i);
                    glBindTexture(GL_TEXTURE_2D, textures[i].id);
                }
            }

            glBindVertexArray(buff.VAO);
            glDrawElementsInstanced(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0, metadata.entities.size());
            glBindVertexArray(0);

            if (!isShadowMapRender)
            {
                for (GLuint i = 0; i < textures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }
    }

    void Renderer::renderInstancedEntities(Camera *camera)
    {
        instancedEntityShader->useShaderProgram();
        instancedEntityShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        instancedEntityShader->loadMatrix("projectionMatrix", camera->getProjectionMatrix());
        instancedEntityShader->loadValue("fogDensity", fogDensity);
        instancedEntityShader->loadVector("fogColor", fogColor);
        loadLightsToShader(instancedEntityShader);
        loadShadowMaps(instancedEntityShader);
        for (auto &pair : instancedEntities)
            renderInstancedModels(pair.first, pair.second, false);
    }

    void Renderer::renderModels(Model3D *model, Shader *shader, bool isShadowMapRender)
    {
        std::vector<Entity *> &v = entities[model];

        for (auto &meshPair : model->meshes)
        {
            const std::string &name = meshPair.first;
            Mesh *mesh = meshPair.second;
            Buffers buff = mesh->getBuffers();
            std::vector<Texture> &textures = mesh->textures;

            if (!isShadowMapRender)
            {
                for (GLuint i = 0; i < textures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    entityShader->loadValue(textures[i].type.c_str(), i);
                    glBindTexture(GL_TEXTURE_2D, textures[i].id);
                }
            }

            glBindVertexArray(buff.VAO);
            for (auto entity : v)
            {
                glm::mat4 translate, rotate, scale;
                computeEntityModelMatrix(entity, translate, rotate, scale);
                getEntityAnimationMatrices(entity->getSubComponentAnimation(name), translate, rotate, scale);
                glm::mat4 modelMatrix = translate * rotate * scale;
                shader->loadMatrix("modelMatrix", modelMatrix);
                if (!isShadowMapRender)
                {
                    shader->loadValue("ambientStrength", entity->ambientStrength);
                    shader->loadValue("specularStrength", entity->specularStrength);
                }
                glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
            }
            glBindVertexArray(0);

            if (!isShadowMapRender)
            {
                for (GLuint i = 0; i < textures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }
    }

    void Renderer::renderEntities(Camera *camera)
    {
        entityShader->useShaderProgram();
        entityShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        entityShader->loadMatrix("projectionMatrix", camera->getProjectionMatrix());
        entityShader->loadValue("fogDensity", fogDensity);
        entityShader->loadVector("fogColor", fogColor);
        loadLightsToShader(entityShader);
        loadShadowMaps(entityShader);
        for (auto &pair : entities)
            renderModels(pair.first, entityShader, false);
    }

    void Renderer::renderDirectionalAndSpotLightShadowMapEntities(Light &light, ShadowMap *map)
    {
        glViewport(0, 0, map->getShadowWidth(), map->getShadowHeight());
        light.calculateLightMatrices(map->getShadowWidth(), map->getShadowHeight());
        directionalAndSpotShadowShader->useShaderProgram();
        directionalAndSpotShadowShader->loadMatrix("lightSpaceMatrix", light.getLightMatrix());
        map->bindFrameBufferObject();
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto &pair : entities)
            renderModels(pair.first, directionalAndSpotShadowShader, true);
        instancedDirectionalAndSpotShadowShader->useShaderProgram();
        instancedDirectionalAndSpotShadowShader->loadMatrix("lightSpaceMatrix", light.getLightMatrix());
        for (auto &pair : instancedEntities)
            renderInstancedModels(pair.first, pair.second, true);
        map->unbindFrameBufferObject();
    }

    void Renderer::renderPointLightShadowMapEntities(PointLight &light, ShadowMap *map)
    {
        glViewport(0, 0, map->getShadowWidth(), map->getShadowHeight());
        light.calculateLightMatrices(map->getShadowWidth(), map->getShadowHeight());
        pointShadowShader->useShaderProgram();
        pointShadowShader->loadVector("lightPosition", light.lightPosition);
        for (int i = 0; i < 6; i++)
            pointShadowShader->loadMatrix("lightSpaceMatrices[" + std::to_string(i) + "]", light.getLightMatrix(i));
        map->bindFrameBufferObject();
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto &pair : entities)
            renderModels(pair.first, pointShadowShader, true);
        instancedPointShadowShader->useShaderProgram();
        instancedPointShadowShader->loadVector("lightPosition", light.lightPosition);
        for (int i = 0; i < 6; i++)
            instancedPointShadowShader->loadMatrix("lightSpaceMatrices[" + std::to_string(i) + "]", light.getLightMatrix(i));
        for (auto &pair : instancedEntities)
            renderInstancedModels(pair.first, pair.second, true);
        map->unbindFrameBufferObject();
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
        quad->getModel()->draw(shadowMapShader);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::renderShadowMaps()
    {
        for (auto &pair : directionalLights)
            if (pair.first.isShadowCasting)
                renderDirectionalAndSpotLightShadowMapEntities(pair.first, pair.second);
        for (int i = 0; i < pointLights.size() / 2; i++)
            if (pointLights[i].first.isShadowCasting)
                renderPointLightShadowMapEntities(pointLights[i].first, pointLights[i].second);
        for (int i = 0; i < spotLights.size() / 2; i++)
            if (spotLights[i].first.isShadowCasting)
                renderDirectionalAndSpotLightShadowMapEntities(spotLights[i].first, spotLights[i].second);
    }

    void Renderer::displayDirectionalAndSpotLightShadowMap(bool spotLight, Entity *quad, int index)
    {
        if (spotLight)
        {
            renderDirectionalAndSpotLightShadowMapEntities(spotLights[index].first, spotLights[index].second);
            __renderShadowMap(quad, spotLights[index].second);
        }
        else
        {
            renderDirectionalAndSpotLightShadowMapEntities(directionalLights[index].first, directionalLights[index].second);
            __renderShadowMap(quad, directionalLights[index].second);
        }
    }

    void Renderer::render(Camera *camera)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        prepareInstances();
        renderShadowMaps();
        glViewport(0, 0, window->getWindowDimensions().width, window->getWindowDimensions().height);
        renderEntities(camera);
        renderInstancedEntities(camera);
        if (enableSkyBox)
            (*skyBox)->render(skyboxShader, camera->getViewMatrix(), camera->getProjectionMatrix(), skyBoxColorModifier);
    }

    void Renderer::renderEntitiesWithFaces(Camera *camera)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window->getWindowDimensions().width, window->getWindowDimensions().height);

        facesShader->useShaderProgram();
        facesShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        facesShader->loadMatrix("projectionMatrix", camera->getProjectionMatrix());
        for (auto &pair : entities)
            renderModels(pair.first, facesShader, true);

        instancedFacesShader->useShaderProgram();
        instancedFacesShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        instancedFacesShader->loadMatrix("projectionMatrix", camera->getProjectionMatrix());
        prepareInstances();
        for (auto &pair : instancedEntities)
            renderInstancedModels(pair.first, pair.second, true);
    }

    void Renderer::addEntity(Entity *entity, bool instanced)
    {
        if (instanced)
        {
            if (entity->getAnimatedSubComponentsCount() > 0)
                throw std::runtime_error(std::string("Entities with subcomponent animation cannot") +
                                         std::string("be used with the instanced rendering pipeline"));
            addInstancedEntity(entity);
        }
        else
            entities[entity->getModel()].push_back(entity);
    }

    void Renderer::cleanup()
    {
        for (auto &instancedPair : instancedEntities)
        {
            ModelMetadata &metadata = instancedPair.second;
            delete[] metadata.instanceData;
            glDeleteBuffers(1, &metadata.vbo);
        }
    }

    DirectionalLight &Renderer::getDirectionalLight(int index)
    {
        return directionalLights[index].first;
    }

    PointLight &Renderer::getPointLight(int index)
    {
        return pointLights[index].first;
    }

    SpotLight &Renderer::getSpotLight(int index)
    {
        return spotLights[index].first;
    }
}