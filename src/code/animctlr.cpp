#include <animctlr.hpp>

std::vector<glm::mat4> AnimationController::boneTransform(float tick, int idx) {
    glm::mat4 ident;
    std::vector<glm::mat4> transforms;
    
    _readNodeHeirarchy(tick, model->scene->mRootNode, ident, idx);
    
    for (auto b : model->bones)
        transforms.push_back(b.finalTrans);
    
    return transforms;
}

void AnimationController::_readNodeHeirarchy(float time, const aiNode *node, const glm::mat4 &parent, int idx) {
    std::string nodeName(node->mName.C_Str());
    aiAnimation* animation = model->scene->mAnimations[idx];
    glm::mat4 nodeTransform = util::assimpToGlmMatrix4x4(node->mTransformation);
    
    const aiNodeAnim *nodeAnim = _findNodeAnim(animation, nodeName);
    
    if(nodeAnim) {
        glm::quat rot = _calculateInterpolatedRotation(time, nodeAnim);
        glm::vec3 trans = _calculateInterpolatedTranslation(time, nodeAnim);
        glm::vec3 scale = _calculateInterpolatedScaling(time, nodeAnim);
        
        nodeTransform =
            glm::translate(glm::mat4(), trans)
            * glm::mat4_cast(rot)
            * glm::scale(glm::mat4(), scale);
    }
    
    glm::mat4 gt = parent * nodeTransform;
    
    auto it = std::find_if(model->bones.begin(), model->bones.end(), [&nodeName](const Bone &b) { return b.name == nodeName; });
    if(it != model->bones.end())
        it->finalTrans = model->globalTransformation * gt * it->offset;
    
    for (uint i = 0; i < node->mNumChildren; i++)
        _readNodeHeirarchy(time, node->mChildren[i], gt, idx);
}

aiNodeAnim* AnimationController::_findNodeAnim(aiAnimation *node, std::string name) {
    for(uint i = 0; i < node->mNumChannels; i++) {
        if(std::string(node->mChannels[i]->mNodeName.C_Str()) == name)
            return node->mChannels[i];
    }
    
    return NULL;
}

glm::quat AnimationController::_calculateInterpolatedRotation(float time, const aiNodeAnim *node) {
    if(node->mNumRotationKeys == 1)
        return util::assimpToGlmQuart(node->mRotationKeys[0].mValue);
    
    int rotationIdx = -1;
    
    for(uint i = 0; i < node->mNumRotationKeys - 1; i++) {
        if(time <= (float)node->mRotationKeys[i + 1].mTime) {
            rotationIdx = i;
            break;
        }
    }
    assert(rotationIdx != -1);
    
    uint nextRotationIdx = rotationIdx + 1;
    assert (nextRotationIdx < node->mNumRotationKeys);
    
    float delta = node->mRotationKeys[nextRotationIdx].mTime - node->mRotationKeys[rotationIdx].mTime;
    float factor = (time - (float)node->mRotationKeys[rotationIdx].mTime) / delta;
    
    const aiQuaternion start = node->mRotationKeys[rotationIdx].mValue;
    const aiQuaternion end = node->mRotationKeys[nextRotationIdx].mValue;
    aiQuaternion result;
    
    aiQuaternion::Interpolate(result, start, end, factor);
    
    return util::assimpToGlmQuart(result);
}

glm::vec3 AnimationController::_calculateInterpolatedTranslation(float time, const aiNodeAnim *node) {
    if(node->mNumPositionKeys == 1)
        return util::assimpToGlmVec3(node->mPositionKeys[0].mValue);
    
    int positionIdx = -1;
    for(uint i = 0; i < node->mNumPositionKeys - 1; i++) {
        if(time <= (float)node->mPositionKeys[i+1].mTime) {
            positionIdx = i;
            break;
        }
    }
    
    assert(positionIdx != -1);
    uint nextPositionIdx = positionIdx + 1;
    assert(nextPositionIdx < node->mNumPositionKeys);
    
    float delta = node->mPositionKeys[nextPositionIdx].mTime - node->mPositionKeys[positionIdx].mTime;
    float factor = (time - (float)node->mPositionKeys[positionIdx].mTime) / delta;
    
    const aiVector3D start = node->mPositionKeys[positionIdx].mValue;
    const aiVector3D end = node->mPositionKeys[nextPositionIdx].mValue;
    aiVector3D result;
    
    Assimp::Interpolator<aiVector3D>()(result, start, end, factor);
    
    return util::assimpToGlmVec3(result);
}

glm::vec3 AnimationController::_calculateInterpolatedScaling(float time, const aiNodeAnim *node) {
    if(node->mNumScalingKeys == 1)
        return util::assimpToGlmVec3(node->mScalingKeys[0].mValue);
    
    int scalingIdx = -1;
    for(uint i = 0; i < node->mNumScalingKeys - 1; i++) {
        if(time <= (float)node->mScalingKeys[i+1].mTime) {
            scalingIdx = i;
            break;
        }
    }
    
    assert(scalingIdx != -1);
    uint nextScalingIdx = scalingIdx + 1;
    assert(nextScalingIdx < node->mNumScalingKeys);
    
    float delta = node->mScalingKeys[nextScalingIdx].mTime - node->mScalingKeys[scalingIdx].mTime;
    float factor = (time - (float)node->mScalingKeys[scalingIdx].mTime) / delta;
    
    const aiVector3D start = node->mScalingKeys[scalingIdx].mValue;
    const aiVector3D end = node->mScalingKeys[nextScalingIdx].mValue;
    aiVector3D result;
    
    Assimp::Interpolator<aiVector3D>()(result, start, end, factor);
    
    return util::assimpToGlmVec3(result);
}