#pragma  once

#include <main.hpp>
#include <map>
#include <vector>



class Pipeline {
public:
    static GLuint createTexture(std::string, bool);
    static std::pair<GLuint, GLuint> createShadowFrame(uint, uint);
    static std::map<GLuint, GLuint> shadowCubeMaps;
    
private:
    static std::map<GLuint, std::string> _textures;
};
