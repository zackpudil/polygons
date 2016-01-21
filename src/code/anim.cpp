#include <anim.hpp>
#include <GLFW/glfw3.h>

Animator::Animator(AnimationController c) : _ctlr(c) {
    _lastTime = glfwGetTime();
    for (uint i = 0 ; i < _ctlr.model->scene->mNumAnimations; i++) {
        aiAnimation *anim = _ctlr.model->scene->mAnimations[i];
        
        _durations.insert({ i, {
            (float)anim->mDuration,
            anim->mTicksPerSecond != 0 ? (float)anim->mTicksPerSecond : 25.0f
        }});
    }
}

Animator& Animator::play(int idx, float start, std::vector<float> end, FrameSkip skip, bool reset) {
    if(end[0] == -1)
        end[0] = _durations[idx].duration;
    
    _loopCallStack.push_back({
        playType,
        idx,
        start,
        end,
        skip,
        reset,
        []{}
    });
    
    return *this;
}

Animator& Animator::loop(int idx, float start, float end, FrameSkip skip, bool reset) {
    _loopCallStack.push_back({
        loopType,
        idx,
        start,
        { end == -1 ? _durations[idx].duration : end },
        skip,
        reset,
        []{}
    });
    
    return *this;
}

Animator& Animator::doWhile(std::function<void ()> func) {
    _loopCallStack[_loopCallStack.size()-1].doWhile = func;
    return *this;
}

void Animator::run(Shader *s) {
    CallStackEntry entryToPlay;
    
    if(_previousCallStack != _loopCallStack) {
        _currentStackIndex = 0;
        _lastStackIndex = -1;
        _currentStackAnimationDone = false;
    }
    
    if(_currentStackAnimationDone) {
        _currentStackIndex += 1;
        _currentStackAnimationDone = false;
        
        if(_currentStackIndex == _loopCallStack.size()) {
            _currentStackIndex -= 1;
            _currentStackAnimationDone = _loopCallStack[_currentStackIndex].type == playType;
        }
        
        entryToPlay = _loopCallStack[_currentStackIndex];
    } else
        entryToPlay = _loopCallStack[_currentStackIndex];
    
    
    if(_lastStackIndex != _currentStackIndex && entryToPlay.resetTime)
        _runningTime = 0;
    
    float frame = _getFrame(entryToPlay.index);

    if(entryToPlay.type == playType)
        _runPlayEntry(entryToPlay, frame, s, true);
    else
        _runLoopEntry(entryToPlay, frame, s, true);
    
    _lastStackIndex = _currentStackIndex;
    
    _previousCallStack.clear();
    _previousCallStack = std::vector<CallStackEntry>(_loopCallStack);
    
    _loopCallStack.clear();
}

void Animator::get(Shader* s) {
    
    auto entryToPlay =
        _loopCallStack.size() == 0 || _currentStackIndex >= _loopCallStack.size()
            ? _previousCallStack[_currentStackIndex]
            : _loopCallStack[_currentStackIndex];
    
    float frame = std::fmod(_durations[entryToPlay.index].framesPerSecond*_runningTime, _durations[entryToPlay.index].duration);
    
    if(entryToPlay.type == playType)
        _runPlayEntry(entryToPlay, frame, s, false);
    else
        _runLoopEntry(entryToPlay, frame, s, false);
}

float Animator::_getFrame(int idx) {
    float currentTime = glfwGetTime();
    
    _runningTime += currentTime - _lastTime;
    _lastTime = currentTime;
    
    return std::fmod(_durations[idx].framesPerSecond*_runningTime, _durations[idx].duration);
}

void Animator::_bindUniforms(std::vector<glm::mat4> t, Shader* shader) {
    for(uint i = 0; i < t.size(); i++)
        shader->bind("bones["+std::to_string(i)+"]", t[i]);
}

void Animator::_runPlayEntry(CallStackEntry entry, float frame, Shader *s, bool runCBs) {
    if(_currentStackAnimationDone) {
        _bindUniforms(_ctlr.boneTransform(_lastFrame, entry.index), s);
        return;
    }
    
    if(frame < entry.startFrame) {
        frame = entry.startFrame;
        _runningTime = entry.startFrame/_durations[entry.index].framesPerSecond;
    }
    
    if(entry.skipFrame != FrameSkip()) {
        if(fabs(frame - entry.skipFrame.from) < 1) {
            frame = entry.skipFrame.to;
            _runningTime = frame/_durations[entry.index].framesPerSecond;
        }
    }
    
    auto it = std::find_if(entry.endFrame.begin(), entry.endFrame.end(), [&frame](float e) { return fabs(frame - e) < 1; });
    
    if (it != entry.endFrame.end()) {
        _currentStackAnimationDone = true;
        _lastFrame = *it;
        _bindUniforms(_ctlr.boneTransform(_lastFrame, entry.index), s);
        
        return;
    }
    
    _bindUniforms(_ctlr.boneTransform(frame, entry.index), s);
    if(runCBs)
        entry.doWhile();
}

void Animator::_runLoopEntry(CallStackEntry entry, float frame, Shader *s, bool runCBs) {
    
    if(frame < entry.startFrame || frame > entry.endFrame[0]) {
        frame = entry.startFrame;
        _runningTime = entry.startFrame/_durations[entry.index].framesPerSecond;
    }
    
    if(entry.skipFrame != FrameSkip()) {
        if(fabs(frame - entry.skipFrame.from) < 1) {
            frame = entry.skipFrame.to;
            _runningTime = frame/_durations[entry.index].framesPerSecond;
        }
    }
    
    _bindUniforms(_ctlr.boneTransform(frame, entry.index), s);
    if(runCBs)
        entry.doWhile();
}