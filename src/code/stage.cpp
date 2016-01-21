#include <stage.hpp>

Stage::Stage(std::string filename, Shader* s, Shader* ss, const float sc) :
    scale(sc), _model(filename), _shader(s), _shadowShader(ss)
{
    for(uint i = 0; i < _model.scene->mNumLights; i++) {
        _processLight(_model.scene->mLights[i]);
    }
}

void Stage::draw(glm::mat4 projection, glm::mat4 view) {
    glm::mat4 model = _getModelMatrix();
    
    _shader->bind("matrix.projection", projection)
        .bind("matrix.view", view)
        .bind("matrix.model", model)
        .bind("castShadow", true);
    
    glStencilMask(0x00);
    _model.draw(_shader);
}

void Stage::drawShadow(std::vector<glm::mat4> shadowTrans, glm::vec3 lightPos, float far_plane) {
    glm::mat4 model = _getModelMatrix();
    
    _shadowShader->bind("model", model)
        .bind("lightPos", lightPos)
        .bind("far_plane", far_plane);
    
    for(uint i = 0; i < shadowTrans.size(); i++)
        _shadowShader->bind("shadowTransforms[" + std::to_string(i) + "]", shadowTrans[i]);
    
    _model.draw(_shadowShader, false);
}

void Stage::_processLight(aiLight *light) {
    aiNode * lightNode = _model.scene->mRootNode->FindNode(light->mName);
    
    glm::vec3 ambient = _findAmbient();
    glm::vec3 diffuse = util::assimpColorToGlmVec3(light->mColorDiffuse);
    glm::vec3 specular = util::assimpColorToGlmVec3(light->mColorSpecular);
    
    aiVector3D aiPosition;
    aiQuaternion aiRotation;
    
    lightNode->mTransformation.DecomposeNoScaling(aiRotation, aiPosition);
    
    glm::mat4 rotateYUp = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(-1, 0, 0));
    glm::vec4 p = util::assimpToGlmVec4(aiPosition);
    glm::vec4 d = util::assimpToGlmVec4(aiRotation.GetMatrix()*light->mDirection);
    
    glm::vec3 position = glm::vec3(rotateYUp*p);
    glm::vec3 direction = glm::vec3(rotateYUp*d);
   
    
    if(light->mType == aiLightSource_DIRECTIONAL) {
        
        lights.addDirectionalLight(direction, ambient, diffuse, specular);
        
    } else if(light->mType == aiLightSource_POINT) {
        
        lights.addPointLight(position,
                              light->mAttenuationLinear,
                              light->mAttenuationQuadratic,
                              ambient, diffuse, specular);
        
    } else if (light->mType == aiLightSource_SPOT) {
        
        lights.addSpotLight(position, direction,
                            light->mAttenuationLinear,
                            light->mAttenuationQuadratic,
                            cos(light->mAngleInnerCone),
                            cos(light->mAngleOuterCone),
                            ambient, diffuse, diffuse);
    }
    
}


glm::vec3 Stage::_findAmbient() {
    glm::vec3 ambient;
    for(uint i = 0; i < _model.scene->mNumLights; i++) {
        aiLight* light = _model.scene->mLights[i];
        if(light->mType == aiLightSource_AMBIENT)
            ambient += util::assimpColorToGlmVec3(light->mColorAmbient);
    }
    
    return ambient;
}

glm::mat4 Stage::_getModelMatrix() {
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    model = glm::scale(model, glm::vec3(scale));
    
    return model;
}