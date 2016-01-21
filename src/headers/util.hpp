#pragma once

#include <main.hpp>
#include <glm/gtc/quaternion.hpp>

class util {
public:
    static glm::mat4 assimpToGlmMatrix4x4(aiMatrix4x4);
    static glm::vec3 assimpToGlmVec3(aiVector3D);
    static glm::vec4 assimpToGlmVec4(aiVector3D);
    static glm::vec3 assimpColorToGlmVec3(aiColor3D);
    
    static glm::vec2 assimpToGlmVec2(aiVector2D);
    static glm::vec2 assimpToGlmVec2(aiVector3D);
    
    static glm::quat assimpToGlmQuart(aiQuaternion);
};