#include <util.hpp>

glm::mat4 util::assimpToGlmMatrix4x4(aiMatrix4x4 mat) {
    return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,
                     mat.a2, mat.b2, mat.c2, mat.d2,
                     mat.a3, mat.b3, mat.c3, mat.d3,
                     mat.a4, mat.b4, mat.c4, mat.d4);
    
}

glm::vec3 util::assimpToGlmVec3(aiVector3D vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec4 util::assimpToGlmVec4(aiVector3D vec) {
    return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}

glm::vec3 util::assimpColorToGlmVec3(aiColor3D col) {
    return glm::vec3(col.r, col.g, col.b);
}

glm::vec2 util::assimpToGlmVec2(aiVector2D vec) {
    return glm::vec2(vec.x, vec.y);
}

glm::vec2 util::assimpToGlmVec2(aiVector3D vec) {
    return glm::vec2(vec.x, vec.y);
}

glm::quat util::assimpToGlmQuart(aiQuaternion quat) {
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
}