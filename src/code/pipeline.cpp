#define STB_IMAGE_IMPLEMENTATION
#include <pipeline.hpp>

std::map<GLuint, std::string> Pipeline::_textures
    = std::map<GLuint, std::string>();

std::map<GLuint, GLuint> Pipeline::shadowCubeMaps = std::map<GLuint, GLuint>();

GLuint Pipeline::createTexture(std::string texturePath, bool alpha) {
    for(auto &x : _textures)
        if (x.second == texturePath)
            return x.first;
    
    int x, y, comp;
    auto image = stbi_load(texturePath.c_str(), &x, &y, &comp, alpha ? STBI_rgb_alpha : STBI_rgb);
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    
    
    GLuint format = alpha ? GL_RGBA : GL_RGB;
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(image);
    
    _textures.insert(std::pair<GLuint, std::string>(textureId, texturePath));
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureId;
}

std::pair<GLuint, GLuint> Pipeline::createShadowFrame(uint w, uint h) {
    GLuint cubeMap;
    glGenTextures(1, &cubeMap);
    
    GLuint depthBuffer;
    glGenFramebuffers(1, &depthBuffer);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    for(GLuint i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    shadowCubeMaps.insert({ cubeMap, depthBuffer });
    
    return { cubeMap, depthBuffer };
}