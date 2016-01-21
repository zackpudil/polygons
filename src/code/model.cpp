#include <model.hpp>
#include <pipeline.hpp>
#include <util.hpp>

void Model::draw(Shader* shader, bool material, bool shadows) {
    for(auto &mesh : _meshes)
        mesh.draw(shader, material, shadows);
}

void Model::_loadModel(std::string filename) {
    auto path = PROJECT_SOURCE_DIR "/resources/models/" + filename;
    scene = _importer.ReadFile(path, aiProcess_Triangulate |  aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "ERROR:ASSIMP::%s", _importer.GetErrorString());
        throw std::exception();
    }
    
    _directory = path.substr(0, path.find_last_of('/')) + '/';
    
    globalTransformation = util::assimpToGlmMatrix4x4(scene->mRootNode->mTransformation.Inverse());
    this->_processNode(scene->mRootNode);
}

void Model::_processNode(aiNode *node) {
    for(uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(_processMesh(mesh));
    }
    
    for(uint i = 0; i < node->mNumChildren; i++)
        _processNode(node->mChildren[i]);
}

Mesh Model::_processMesh(aiMesh *mesh) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    for(uint i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        
        vertex.position = util::assimpToGlmVec3(mesh->mVertices[i]);
        vertex.normal = util::assimpToGlmVec3(mesh->mNormals[i]);
        vertex.tex = util::assimpToGlmVec2(mesh->mTextureCoords[0][i]);
        vertex.tangent = util::assimpToGlmVec3(mesh->mTangents[i]);
        
        vertices.push_back(vertex);
    }
    
    for(uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(uint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    auto bones = _loadBones(mesh->mBones, mesh->mNumBones);
    
    for(auto b : bones) {
        for (auto p : b.weights) {
            Vertex* v = &vertices[p.first];
            
            v->boneIds[v->boneAndWeightSize] = b.idx;
            v->weights[v->boneAndWeightSize] = p.second;
            v->boneAndWeightSize += 1;
        }
    }
    
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    
    std::vector<Texture> diffuseMaps = _loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    
    std::vector<Texture> specularMaps = _loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
    std::vector<Texture> normalMaps = _loadMaterialTextures(material, aiTextureType_NORMALS, "normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    
    float shinines;
    material->Get(AI_MATKEY_SHININESS, shinines);
    shinines = shinines == 0 ? 32 : shinines;
    
    return Mesh(vertices, indices, textures, (int)shinines);
}

std::vector<Bone> Model::_loadBones(aiBone **aiBones, uint num) {
    
    std::vector<Bone> vertexBoneData;
    
    for(uint i = 0; i < num; i++) {
        Bone b;
        aiBone* aiB = aiBones[i];
        std::string name(aiB->mName.C_Str());
        
        auto foundBone = std::find_if(bones.begin(), bones.end(), [&name](const Bone& _b) { return _b.name == name; });
        
        if(foundBone == bones.end()) {
            b.idx = bones.size();
            b.name = name;
            
            for (uint j = 0; j < aiB->mNumWeights; j++)
                b.weights.push_back({ aiB->mWeights[j].mVertexId, aiB->mWeights[j].mWeight });
            
            b.offset = util::assimpToGlmMatrix4x4(aiB->mOffsetMatrix);
            
            bones.push_back(b);
            vertexBoneData.push_back(b);
        } else
            vertexBoneData.push_back(*foundBone);
    }
    
    return vertexBoneData;
}

std::vector<Texture> Model::_loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    
    for(uint i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        
        Texture texture = {
            Pipeline::createTexture(_directory + std::string(str.C_Str()), false),
            typeName,
            std::string(str.C_Str())
        };
        
        textures.push_back(texture);
    }
    
    return textures;
}