#include <graphics/Renderer.hpp>
#include <engine/ErrorCheck.hpp>

namespace gps
{
    Renderer::Renderer(Window *window) : window(window)
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
    }

    void Renderer::addEntity(Entity *entity)
    {
        entities[entity->getModel()].push_back(entity);
    }

    void Renderer::renderModels(Model3D *model, Shader *shader)
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
                glUniform1i(glGetUniformLocation(shader->shaderProgram, textures[i].type.c_str()), i);
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
                shader->loadMatrix("modelMatrix", modelMatrix);
                shader->loadValue("ambientStrength", entity->ambientStrength);
                shader->loadValue("specularStrength", entity->specularStrength);
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

    void Renderer::renderEntities(Camera *camera, Shader *shader, glm::mat4 projectionMatrix, ShadowMap *shadow)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->useShaderProgram();
        shader->loadMatrix("viewMatrix", camera->getViewMatrix());
        shader->loadMatrix("projectionMatrix", projectionMatrix);
        shader->loadMatrix("lightSpaceMatrix", directionalLights[0].lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE31);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, shadow->getTextureID());
        glCheckError();
        shader->loadValue("shadowMap", 31);
        for (int i = 0; i < directionalLights.size(); i++)
            directionalLights[i].loadUniforms(shader, i);
        for (int i = 0; i < pointLights.size(); i++)
            pointLights[i].loadUniforms(shader, i);
        for (int i = 0; i < spotLights.size(); i++)
            spotLights[i].loadUniforms(shader, i);

        for (auto &pair : entities)
            renderModels(pair.first, shader);
    }

    void Renderer::renderDirectionalLightShadowMapModels(Model3D *model, Shader *shader)
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

    void Renderer::renderDirectionalLightShadowMapEntities(int index, Shader *shader, ShadowMap *shadowMap)
    {
        shader->useShaderProgram();
        directionalLights[0].calculateLightSpaceMatrix();
        shader->loadMatrix("lightSpaceMatrix", directionalLights[0].lightSpaceMatrix);

        glViewport(0, 0, shadowMap->shadowWidth, shadowMap->shadowHeight);
        shadowMap->bindFrameBufferObject();

        glClear(GL_DEPTH_BUFFER_BIT);

        for (auto &pair : entities)
            renderDirectionalLightShadowMapModels(pair.first, shader);

        shadowMap->unbindFrameBufferObject();
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
    
    void Renderer::renderShadowMap(Entity *quad, Shader *shader, ShadowMap *shadowMap)
    {
        glViewport(0, 0, window->getWindowDimensions().width, window->getWindowDimensions().height);
		glClear(GL_COLOR_BUFFER_BIT);
		shader->useShaderProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowMap->getTextureID());
        shader->loadValue("shadowMap", 0);
		glDisable(GL_DEPTH_TEST);
	    quad->getModel()->Draw(shader);
		glEnable(GL_DEPTH_TEST);
    }
}