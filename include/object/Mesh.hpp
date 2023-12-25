#pragma once

#include <engine/GeneralIncludes.hpp>
#include <graphics/Shader.hpp>

#include <string>
#include <vector>

namespace gps
{

    struct Vertex
    {

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture
    {

        GLuint id;
        // ambientTexture, diffuseTexture, specularTexture
        std::string type;
        std::string path;
    };

    struct Material
    {

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct Buffers
    {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };

    class Mesh
    {

    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures);

        Buffers getBuffers();
        void draw(gps::Shader *shader);

    private:
        Buffers buffers;

        void setupMesh();
    };

}
