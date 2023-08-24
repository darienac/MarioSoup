#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "audio/AudioBuffer.h"

class AudioSource {
    private:
    ALuint sourceId;

    public:
    AudioSource(bool loop, bool relative) {
        alGenSources(1, &sourceId);
        alSourcei(sourceId, AL_LOOPING, loop);
        alSourcei(sourceId, AL_SOURCE_RELATIVE, relative);

        setMaxDistance(1.0f);
        setReferenceDistance(1.0f);

        setPos(0.0f, 0.0f, 0.0f);
        setVel(0.0f, 0.0f, 0.0f);
        setDir(0.0f, 0.0f, 1.0f);
    }

    void setPos(float x, float y, float z) {
        alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
    }

    void setVel(float dx, float dy, float dz) {
        alSource3f(sourceId, AL_VELOCITY, dx, dy, dz);
    }

    void setDir(float dx, float dy, float dz) {
        alSource3f(sourceId, AL_DIRECTION, dx, dy, dz);
    }

    void setMaxDistance(float value) {
        alSourcef(sourceId, AL_MAX_DISTANCE, value);
    }

    void setReferenceDistance(float value) {
        alSourcef(sourceId, AL_REFERENCE_DISTANCE, value);
    }

    void playBuffer(AudioBuffer* buffer) {
        stopBuffer();
        ALuint bufferId = buffer->getBufferId();
        alSourcei(sourceId, AL_BUFFER, bufferId);
        alSourcePlay(sourceId);
    }

    void stopBuffer() {
        alSourceRewind(sourceId);
        alSourcei(sourceId, AL_BUFFER, 0);
    }

    bool isPlaying() {
        ALint state;
        alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    ~AudioSource() {
        alDeleteSources(1, &sourceId);
    }
};