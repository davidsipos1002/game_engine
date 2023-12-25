#pragma once

#include <engine/GeneralIncludes.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>

namespace gps
{

    class Shader
    {

    public:
        ~Shader();
        
        GLuint shaderProgram;
        void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
        void loadShader(const std::string &vertexShaderFileName, const std::string &geometryShaderFileName, const std::string &fragmentShaderFileName);
        void useShaderProgram();
        void loadVector(const std::string &uniformName, const glm::vec3 &vector);
        void loadVector(const std::string &uniformName, const glm::vec4 &vector);
        void loadMatrix(const std::string &uniformName, const glm::mat3 &matrix);
        void loadMatrix(const std::string &uniformName, const glm::mat4 &matrix);
        void loadValue(const std::string &uniformName, int value);
        void loadValue(const std::string &uniformName, float value);
        void loadValue(const std::string &uniformName, GLuint value);

    private:
        std::string readShaderFile(const std::string &fileName);
        void shaderCompileLog(GLuint shaderId);
        void shaderLinkLog(GLuint shaderProgramId);
    };

}
