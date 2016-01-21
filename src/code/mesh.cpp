#include <mesh.hpp>

void Mesh::draw(Shader* shader, bool materials, bool shadows) {

    if(materials) {
        
        if(shadows) {
            auto it = Pipeline::shadowCubeMaps.begin();
            for(uint i = 0; i < Pipeline::shadowCubeMaps.size(); i++) {
                auto d = *it;
                glActiveTexture(GL_TEXTURE0 + (d.first - 1));
                glBindTexture(GL_TEXTURE_CUBE_MAP, d.first);
                shader->bind("shadowMap["+std::to_string(i)+"]", (int)d.first - 1);
                it++;
            }
        }
        
        shader->bind("material.shininess", shininess*4);
        
        GLuint diffuseNr = 0;
        GLuint specularNr = 0;
        GLuint normalNr = 0;
        
        for(GLuint i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0+textures[i].id-1);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            
            std::stringstream ss;
            std::string number;
            std::string name = textures[i].type;
            
            if(name == "diffuse")
                ss << diffuseNr++;
            else if(name == "specular")
                ss << specularNr++;
            else if(name == "normal")
                ss << normalNr++;
            
            number = ss.str();
            
            shader->bind("material."+ name + "[" + number + "]", (int)textures[i].id - 1);
        }
        
        if(normalNr == 0)
            shader->bind("material.hasNormals", false);
        else
            shader->bind("material.hasNormals", true);
    }
    
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::_setupMesh() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    
    glBindVertexArray(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex));
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
    
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, boneIds));
    
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, weights));
    
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void *)offsetof(Vertex, boneAndWeightSize));
    
    glBindVertexArray(0);
}