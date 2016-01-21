#pragma once

#include <main.hpp>
#include <animctlr.hpp>
#include <map>
#include <functional>

struct AnimationDuration {
    float duration;
    float framesPerSecond;
};

enum AnimatorMethodType {
    playType,
    loopType
};

struct FrameSkip {
    float from;
    float to;
    
    bool operator==(const FrameSkip& rhs) const {
        return this->from == rhs.from && this->to == rhs.to;
    }
    
    bool operator!=(const FrameSkip& rhs) const {
        return !(*this == rhs);
    }
};

struct CallStackEntry {
    AnimatorMethodType type;
    int index;
    float startFrame;
    std::vector<float> endFrame;
    FrameSkip skipFrame;
    bool resetTime;
    
    std::function<void()> doWhile;
    
    bool operator==(const CallStackEntry& rhs) const {
        return this->type == rhs.type
            && this->index == rhs.index
            && this->startFrame == rhs.startFrame
            && this->endFrame == rhs.endFrame
            && this->skipFrame == rhs.skipFrame
            && this->resetTime == rhs.resetTime;
    }
    
    bool operator!=(const CallStackEntry& rhs) const {
        return !(*this == rhs);
    }
};

class Animator {
public:
    Animator(AnimationController);
    
    Animator& play(int, float start = 0, std::vector<float> end = {-1}, FrameSkip skip = {}, bool reset = false);
    Animator& loop(int, float start = 0, float end = -1, FrameSkip skip = {}, bool reset = true);
    
    Animator& doWhile(std::function<void()>);
    
    void run(Shader* _shader);
    void get(Shader* _shader);
    
private:
    AnimationController _ctlr;
    std::map<int, AnimationDuration> _durations;
    
    float _runningTime = 0;
    float _lastTime = 0;
    
    uint _currentStackIndex = 0;
    uint _lastStackIndex = 0;
    float _lastFrame = 0;
    bool _currentStackAnimationDone = false;

    std::vector<CallStackEntry> _loopCallStack;
    std::vector<CallStackEntry> _previousCallStack;
    
    float _getFrame(int);
    void _bindUniforms(std::vector<glm::mat4>, Shader*);
    
    void _runPlayEntry(CallStackEntry, float, Shader*, bool);
    void _runLoopEntry(CallStackEntry, float, Shader*, bool);
};