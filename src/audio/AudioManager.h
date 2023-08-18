#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <set>

#include "audio/AudioBuffer.h"
#include "audio/AudioSource.h"
#include "audio/AudioCache.h"

class AudioManager {
    private:
    const ALfloat pos[3] = {0.0, 0.0, 0.0};
    const ALfloat vel[3] = {0.0, 0.0, 0.0};
    const ALfloat ori[6] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

    ALCdevice* device;
    ALCcontext* context;

    AudioSource* musicSource;
    std::set<AudioSource*> soundSources;

    float soundMaxDistance = 1.0;
    float referenceDistance = 1.0;

    public:
    AudioManager() {
        #ifndef MUTE
        device = alcOpenDevice(NULL);
        if (device == NULL) {
            throw "Unable to use default OpenAL device";
        }

        context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
        AudioCache::init();

        setPos(pos);
        alListenerfv(AL_VELOCITY, vel);
        alListenerfv(AL_ORIENTATION, ori);

        musicSource = new AudioSource(true, false);
        #endif
    }

    void setPos(const float pos[3]) {
        #ifndef MUTE
        alListenerfv(AL_POSITION, pos);
        // std::printf("Listener pos: x: %f, y: %f, z: %f\n", pos[0], pos[1], pos[2]);
        #endif
    }

    void cancelMusic() {
        #ifndef MUTE
        musicSource->cancelBuffers();
        #endif
    }

    void setMusic(AudioBuffer& buffer) {
        #ifndef MUTE
        cancelMusic();
        musicSource->playBuffer(buffer);
        #endif
    }

    void playSound(AudioBuffer& buffer, float pos[3]) {
        #ifndef MUTE
        AudioSource* source = new AudioSource(false, false);
        source->setMaxDistance(soundMaxDistance);
        source->setReferenceDistance(referenceDistance);
        source->setPos(pos);
        source->playBuffer(buffer);

        soundSources.insert(source);
        #endif
    }

    void update() {
        #ifndef MUTE
        musicSource->update();
        auto it = soundSources.begin();
        while (it != soundSources.end()) {
            AudioSource* source = *it;
            source->update();
            if (source->isPlaying()) {
                it++;
            } else {
                delete source;
                soundSources.erase(it++);
            }
        }
        #endif
    }
    
    #ifndef MUTE
    ~AudioManager() {
        delete musicSource;
        for (AudioSource* source : soundSources) {
            delete source;
        }

        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
    #endif
};