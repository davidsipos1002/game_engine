#include <engine/GeneralIncludes.hpp>
#include <graphics/Shader.hpp>
#include <exception>

#define GET_LOC(name) glGetUniformLocation(shaderProgram, (name).c_str())
namespace gps
{
    Shader::~Shader() {
        glDeleteProgram(shaderProgram);
    }

    std::string Shader::readShaderFile(const std::string &fileName)
    {
        std::ifstream shaderFile;
        std::string shaderString;
        shaderFile.open(fileName);
        std::stringstream shaderStringStream;
        shaderStringStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderString = shaderStringStream.str();
        return shaderString;
    }

    void Shader::shaderCompileLog(GLuint shaderId)
    {
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "Shader compilation error\n"
                      << infoLog << std::endl;
        }
    }

    void Shader::shaderLinkLog(GLuint shaderProgramId)
    {
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "Shader linking error\n"
                      << infoLog << std::endl;
        }
    }

    void Shader::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {
        std::string v = readShaderFile(vertexShaderFileName);
        const GLchar *vertexShaderString = v.c_str();
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
        glCompileShader(vertexShader);
        shaderCompileLog(vertexShader);

        std::string f = readShaderFile(fragmentShaderFileName);
        const GLchar *fragmentShaderString = f.c_str();
        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
        glCompileShader(fragmentShader);
        shaderCompileLog(fragmentShader);

        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, vertexShader);
        glAttachShader(this->shaderProgram, fragmentShader);
        glLinkProgram(this->shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        shaderLinkLog(this->shaderProgram);
    }
    
    void Shader::loadShader(const std::string &vertexShaderFileName, const std::string &geometryShaderFileName, const std::string &fragmentShaderFileName)
    {
        std::string v = readShaderFile(vertexShaderFileName);
        const GLchar *vertexShaderString = v.c_str();
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
        glCompileShader(vertexShader);
        shaderCompileLog(vertexShader);

        std::string g = readShaderFile(geometryShaderFileName);
        const GLchar *geometryShaderString = g.c_str();
        GLuint geometryShader;
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderString, NULL);
        glCompileShader(geometryShader);
        shaderCompileLog(geometryShader);
        
        std::string f = readShaderFile(fragmentShaderFileName);
        const GLchar *fragmentShaderString = f.c_str();
        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
        glCompileShader(fragmentShader);
        shaderCompileLog(fragmentShader);

        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, vertexShader);
        glAttachShader(this->shaderProgram, geometryShader);
        glAttachShader(this->shaderProgram, fragmentShader);
        glLinkProgram(this->shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
        shaderLinkLog(this->shaderProgram);
    }

    void Shader::useShaderProgram()
    {
        glUseProgram(this->shaderProgram);
    }

    void Shader::loadVector(const std::string &uniformName, const glm::vec3 &vector)
    {
        glUniform3fv(GET_LOC(uniformName), 1, glm::value_ptr(vector));
    }

    void Shader::loadVector(const std::string &uniformName, const glm::vec4 &vector)
    {
        glUniform4fv(GET_LOC(uniformName), 1, glm::value_ptr(vector));
    }
    
    void Shader::loadMatrix(const std::string &uniformName, const glm::mat3 &matrix) 
    {
        glUniformMatrix3fv(GET_LOC(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void Shader::loadMatrix(const std::string &uniformName, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(GET_LOC(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::loadValue(const std::string &uniformName, int value)
    {
        glUniform1i(GET_LOC(uniformName), value);
    }

    void Shader::loadValue(const std::string &uniformName, float value)
    {
        glUniform1f(GET_LOC(uniformName), value);
    }

    void Shader::loadValue(const std::string &uniformName, GLuint value)
    {
        glUniform1i(GET_LOC(uniformName), value);
    }
}