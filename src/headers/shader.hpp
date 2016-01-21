#pragma once

#include <main.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

#include <material.hpp>

class Shader {
public:
    Shader();
    ~Shader();

    Shader& attach(std::string const & filename);
    
    Shader& link();
    Shader& activate();
    
    void bind(GLuint loc, glm::mat4 value);
    void bind(GLuint loc, glm::vec4 value);
    void bind(GLuint loc, glm::vec3 value);
    void bind(GLuint loc, float value);
    
    void bindMaterial(std::string const name, Material value);
    
    template <typename T> Shader& bind(std::string const &name, T&& value) {
        int loc = glGetUniformLocation(_program, name.c_str());
        if(loc == -1) printf("couldn't find uniform: %s\n", name.c_str());
        else bind(loc, std::forward<T>(value));
        
        return *this;
    }

    GLuint get();

private:
    GLuint _program;
    GLuint create(std::string const & filename);
};
