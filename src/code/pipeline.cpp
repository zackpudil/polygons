#define STB_IMAGE_IMPLEMENTATION
#include <pipeline.hpp>

std::map<GLuint, std::vector<float>> Pipeline::_vertexArrayObjects
    = std::map<GLuint, std::vector<float>>();

std::map<GLuint, std::string> Pipeline::_textures
    = std::map<GLuint, std::string>();

GLuint Pipeline::_vbo = 0;

std::vector<glm::vec3> Pipeline::calculateNormals(std::vector<glm::vec3> shape,
                                               std::vector<unsigned int> indices)
{
    std::vector<glm::vec3> actualShape;
    std::vector<glm::vec3> normals;
    
    for(auto &i : indices) {
        actualShape.push_back(shape[i]);
    }
    
    for(int i = 0; i < (int)actualShape.size(); i += 3) {
        glm::vec3 v1 = actualShape[i];
        auto v2 = actualShape[i+1];
        auto v3 = actualShape[i+2];
        
        glm::vec3 e1 = v2 - v1;
        auto e2 = v3 - v1;
        
        for(int j = 0; j < 3; j++)
            normals.push_back(glm::normalize(glm::cross(e1, e2)));
    }
    
    return normals;
}


std::vector<float> Pipeline::createVerts(std::vector<glm::vec3> shape,
                               std::vector<glm::vec3> norms,
                               std::vector<glm::vec2> tex,
                               std::vector<unsigned int> indices)
{
    std::vector<glm::vec3> actualShape;
    
    for(auto &i : indices) {
        actualShape.push_back(shape[i]);
    }
    
    std::vector<float> verts;
    int j = 0;
    
    for(int i = 0; i < (int)actualShape.size(); i++) {
        glm::vec3 vert = actualShape[i];
        glm::vec3 normal = norms[i];
        
        verts.push_back(vert.x);
        verts.push_back(vert.y);
        verts.push_back(vert.z);
        
        verts.push_back(normal.x);
        verts.push_back(normal.y);
        verts.push_back(normal.z);
    
        verts.push_back(tex[j].x);
        verts.push_back(tex[j].y);
        
        j++;
        if(j == (int)tex.size())
            j = 0;
    }
    
    return verts;
}

GLuint Pipeline::createVertexArray(std::vector<float> verts) {
    for (auto &x : _vertexArrayObjects)
        if(x.second == verts) return x.first;
    
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    
    Pipeline::_vertexArrayObjects.insert(std::pair<GLuint, std::vector<float>>(vao, verts));
    Pipeline::_bindBufferData(vao);
    
    return vao;
}

GLuint Pipeline::createTexture(std::string textureName, bool alpha) {
    for(auto &x : _textures)
        if (x.second == textureName)
            return x.first;
    
    std::string path = PROJECT_SOURCE_DIR "/resources/textures/" + textureName;
    
    int x, y, comp;
    auto image = stbi_load(path.c_str(), &x, &y, &comp, alpha ? STBI_rgb_alpha : STBI_rgb);
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    
    
    GLuint format = alpha ? GL_RGBA : GL_RGB;
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(image);
    
    _textures.insert(std::pair<GLuint, std::string>(textureId, textureName));
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureId;
}

void Pipeline::bind(GLuint vao, GLuint textureId) {
    GLint currentVao;
    
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    if(vao == (GLuint)currentVao) return;
    
    glBindVertexArray(vao);
}

void Pipeline::unbind() {
    glBindVertexArray(0);
}


void Pipeline::destroy() {
    glDeleteBuffers(1, &_vbo);
    for(auto &i : _vertexArrayObjects) glDeleteVertexArrays(1, &i.first);
}

void Pipeline::_bindBufferData(GLuint vao) {
    std::vector<float> data = _vertexArrayObjects[vao];
    
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(0,  3, GL_FLOAT, false, sizeof(float)*8, 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float)*8, (void *)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float)*8, (void *)(sizeof(float)*6));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    
}