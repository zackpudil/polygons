#pragma once

#include <main.hpp>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <streambuf>

class Shader {
public:
    Shader();
    ~Shader();

    Shader& attachLibrary(std::string const &);
    Shader& attach(std::string const &);
    
    Shader& link();
    Shader& activate();
    
    void bind(GLuint, glm::mat4);
    void bind(GLuint, glm::vec4);
    void bind(GLuint, glm::vec3);
    void bind(GLuint, float);
    void bind(GLuint, int);
    
    template <typename T> Shader& bind(std::string const &name, T&& value) {
        int loc = glGetUniformLocation(_program, name.c_str());
        if(loc == -1) {
            fprintf(stderr, "couldn't find uniform: %s\n", name.c_str());
            throw std::exception();
        }
        else bind(loc, std::forward<T>(value));
        
        return *this;
    }

    GLuint get();

private:
    GLuint _program;
    std::map<std::string, std::string> _libaries = std::map<std::string, std::string>();
    GLuint create(std::string const & filename);
};
