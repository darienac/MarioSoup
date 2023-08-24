#pragma once

#include <map>
#include <string>
#include <filesystem>

#include "audio/AudioBuffer.h"
#include "ResourceReader.h"

namespace fs = std::filesystem;

namespace AudioCache {
    std::map<std::string, AudioBuffer*> audio;
    std::map<std::string, std::string> music;
};

#ifndef MUTE
namespace {
    AudioBuffer& addAudio(AudioBuffer* object) {
        AudioCache::audio[object->getId()] = object;
        return *object;
    }
    void addAudioBatch(std::string path, std::string prefix) {
        std::string fullPath = ResourceReader::getFullPath(ResourceReader::Audio, path);
        for (auto const& dir_entry : fs::directory_iterator(fullPath)) {
            addAudio(new AudioBuffer(prefix + dir_entry.path().stem().string(), dir_entry));
        }
    }
    void addMusic(std::string id, std::string path) {
        AudioCache::music[id] = path;
    }
}
#endif

namespace AudioCache {
    void init() {
        #ifndef MUTE
        addMusic("testsong", "testsong.ogg");
        addMusic("sma4:overworld", "sma4_overworld.ogg");

        addAudioBatch("smas", "smas:");
        addAudio(new AudioBuffer("smb3:jump", "smb3_jump.ogg"));
        #endif
    }
}