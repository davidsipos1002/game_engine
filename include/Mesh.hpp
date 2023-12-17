#ifndef Mesh_hpp
#define Mesh_hpp

#include <GeneralIncludes.hpp>

#include "Shader.hpp"

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

        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

        Buffers getBuffers();

    private:
        Buffers buffers;

        void setupMesh();
    };

}
#endif /* Mesh_hpp */
