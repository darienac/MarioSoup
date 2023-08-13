#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "audio/AudioBuffer.h"
#include "audio/AudioSource.h"

class AudioManager {
    private:
    const ALfloat pos[3] = {0.0, 0.0, 0.0};
    const ALfloat vel[3] = {0.0, 0.0, 0.0};
    const ALfloat ori[6] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

    ALCdevice* device;
    ALCcontext* context;

    public:
    AudioManager() {
        device = alcOpenDevice(NULL);
        if (device == NULL) {
            throw "Unable to use default OpenAL device";
        }

        context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
        alListenerfv(AL_POSITION, pos);
        alListenerfv(AL_VELOCITY, vel);
        alListenerfv(AL_ORIENTATION, ori);
    }
    
    ~AudioManager() {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
};