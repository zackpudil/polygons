#pragma once

#include <vector>
#include <main.hpp>
#include <model.hpp>
#include <shader.hpp>
#include <util.hpp>


class AnimationController {
public:
    Model* model;
    
    AnimationController(Model* m) : model(m) { }
    std::vector<glm::mat4> boneTransform(float, int);

private:
    void _readNodeHeirarchy(float, const aiNode*, const glm::mat4&, int);
    aiNodeAnim* _findNodeAnim(aiAnimation*, std::string);

    glm::quat _calculateInterpolatedRotation(float, const aiNodeAnim*);
    glm::vec3 _calculateInterpolatedTranslation(float, const aiNodeAnim*);
    glm::vec3 _calculateInterpolatedScaling(float, const aiNodeAnim*);
};
