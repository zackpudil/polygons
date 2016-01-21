#pragma once

#include <main.hpp>
#include <mesh.hpp>
#include <shader.hpp>

struct Bone {
    int idx;
    std::string name;
    
    glm::mat4 offset;
    glm::mat4 finalTrans;
    
    std::vector<std::pair<uint, float>> weights;
};

struct Ellipsoid {
    glm::vec3 center;
    glm::mat3 scale;
};

class Model {
public:
    glm::mat4 globalTransformation;
    const aiScene *scene;
    std::vector<Bone> bones;
    
    Model(std::string filename) {
        this->_loadModel(filename);
    }
    
    void draw(Shader*, bool m = true, bool s = true);
    
    Ellipsoid ellipsoid;
    
private:
    std::vector<Mesh> _meshes;
    std::string _directory;
    Assimp::Importer _importer;
    
    void _loadModel(std::string);
    void _processNode(aiNode*);
    Mesh _processMesh(aiMesh*);
    
    std::vector<Texture> _loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
    std::vector<Bone> _loadBones(aiBone **, uint);
};