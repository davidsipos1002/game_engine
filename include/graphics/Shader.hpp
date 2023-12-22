//
//  Shader.hpp
//  Lab3
//
//  Created by CGIS on 05/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

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
        void loadShader(std::string vertexShaderFileName, std::string fragmentShaderFileName);
        void useShaderProgram();
        void loadVector(std::string uniformName, glm::vec3 vector);
        void loadVector(std::string uniformName, glm::vec4 vector);
        void loadMatrix(std::string uniformName, glm::mat3 matrix);
        void loadMatrix(std::string uniformName, glm::mat4 matrix);
        void loadValue(std::string uniformName, int value);
        void loadValue(std::string uniformName, float value);

    private:
        std::string readShaderFile(std::string fileName);
        void shaderCompileLog(GLuint shaderId);
        void shaderLinkLog(GLuint shaderProgramId);
    };

}

#endif /* Shader_hpp */
