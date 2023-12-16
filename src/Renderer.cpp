#include <Renderer.hpp>
#include <ErrorCheck.hpp>

namespace gps
{
    void Renderer::addEntity(Entity *entity)
    {
        entities[entity->getModel()].push_back(entity);
    }

    void Renderer::renderModels(Model3D *model, Shader *shader)
    {
        std::vector<Entity *> &v = entities[model];

        for (auto &mesh : model->meshes)
        {
            Buffers buff = mesh.getBuffers();
            std::vector<Texture> &textures = mesh.textures;

            for (GLuint i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glUniform1i(glGetUniformLocation(shader->shaderProgram, textures[i].type.c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glBindVertexArray(buff.VAO);

            for (auto entity : v)
            {
                glm::mat4 scale = glm::scale(entity->scale);
                glm::mat4 rotate = glm::rotate(entity->rotation.x, glm::vec3(1.0f, 0, 0));
                rotate *= glm::rotate(entity->rotation.y, glm::vec3(0, 1.0f, 0));
                rotate *= glm::rotate(entity->rotation.z, glm::vec3(0, 0, 1.0f));
                glm::mat4 translate = glm::translate(entity->position);
                glm::mat4 modelMatrix = translate * rotate * scale;

                shader->loadMatrix("modelMatrix", modelMatrix);

                glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, 0);
            }

            glBindVertexArray(0);
            for (GLuint i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }
    
    void Renderer::renderEntities(Camera *camera, Shader *shader, glm::mat4 projectionMatrix)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->useShaderProgram();
        shader->loadMatrix("viewMatrix", camera->getViewMatrix());
        shader->loadMatrix("projectionMatrix", projectionMatrix);
        directionalLight.loadUniforms(shader, 0);

        for (auto &pair : entities) {
            renderModels(pair.first, shader);
        }
    }
}