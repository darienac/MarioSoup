#pragma once

#include <map>
#include <string>

#include "audio/AudioBuffer.h"

namespace AudioCache {
    struct strCompare {
        bool operator()(const char* a, const char* b) const {
            return strcmp(a, b) < 0;
        }
    };
    std::map<const char*, AudioBuffer*, strCompare> audio;
};

namespace {
    AudioBuffer& addAudio(AudioBuffer* object) {
        AudioCache::audio[object->getId()] = object;
        return *object;
    }
}

namespace AudioCache {
    void init() {
        addAudio(new AudioBuffer("smb3:jump", "smb3_jump.ogg"));
        addAudio(new AudioBuffer("sma4:overworld", "sma4_overworld.ogg"));
    }
}