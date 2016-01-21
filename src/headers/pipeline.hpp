	#pragma  once

#include <main.hpp>
#include <map>
#include <vector>

class Pipeline {
public:
    static std::vector<glm::vec3> calculateNormals(std::vector<glm::vec3>,
                                               std::vector<unsigned int>);
    
    static std::vector<float> createVerts(std::vector<glm::vec3>,
                                          std::vector<glm::vec3>,
                                          std::vector<glm::vec2>,
                                          std::vector<unsigned int>);
    static GLuint createVertexArray(std::vector<float>);
    static GLuint createTexture(std::string, bool);
    
    static void bind(GLuint, GLuint);
    static void unbind();
    
    static void destroy();
    
private:
    static GLuint _vbo;
    
    static std::map<GLuint, std::vector<float>> _vertexArrayObjects;
    static std::map<GLuint, std::string> _textures;
    
    static void _bindBufferData(GLuint);
};
