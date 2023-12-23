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
        directionalShadowMap = new ShadowMap(window->getWindowDimensions().width, window->getWindowDimensions().height);
        entityShader = loader->loadShader("shaders/entityNormal.vert", "shaders/entityNormal.frag");
        directionalShadowShader = loader->loadShader("shaders/directionalShadow.vert", "shaders/directionalShadow.frag");
        shadowMapShader = loader->loadShader("shaders/shadowMap.vert", "shaders/shadowMap.frag"); 
    }

    Renderer::~Renderer()
    {
        delete directionalShadowMap;
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
                glActiveTexture(GL_TEXTURE0 + i);
                glUniform1i(glGetUniformLocation(entityShader->shaderProgram, textures[i].type.c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
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
        renderDirectionalLightShadowMapEntities();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        entityShader->useShaderProgram();
        entityShader->loadMatrix("viewMatrix", camera->getViewMatrix());
        entityShader->loadMatrix("projectionMatrix", projectionMatrix);
        directionalLights[directionalShadowCaster].calculateLightMatrices();
        entityShader->loadMatrix("lightSpaceMatrix", directionalLights[directionalShadowCaster].lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE31);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, directionalShadowMap->getTextureID());
        glCheckError();
        entityShader->loadValue("directionalShadowCaster", directionalShadowCaster);
        entityShader->loadValue("directionalShadowMap", 31);
        for (int i = 0; i < directionalLights.size(); i++)
            directionalLights[i].loadUniforms(entityShader, i);
        for (int i = 0; i < pointLights.size(); i++)
            pointLights[i].loadUniforms(entityShader, i);
        for (int i = 0; i < spotLights.size(); i++)
            spotLights[i].loadUniforms(entityShader, i);

        for (auto &pair : entities)
            renderModels(pair.first);
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

    void Renderer::renderDirectionalLightShadowMapEntities()
    {
        directionalShadowShader->useShaderProgram();
        directionalShadowShader->loadMatrix("lightSpaceMatrix", directionalLights[directionalShadowCaster].lightSpaceMatrix);
        glViewport(0, 0, directionalShadowMap->shadowWidth, directionalShadowMap->shadowHeight);
        directionalShadowMap->bindFrameBufferObject();
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto &pair : entities)
            renderShadowMapModels(pair.first, directionalShadowShader);
        directionalShadowMap->unbindFrameBufferObject();
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

    void Renderer::renderDirectionalShadowMap(Entity *quad)
    {
        renderDirectionalLightShadowMapEntities();
        __renderShadowMap(quad, directionalShadowMap);
    }
}