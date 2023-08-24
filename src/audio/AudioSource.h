#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "audio/AudioBuffer.h"

class AudioSource {
    private:
    ALuint sourceId;
    bool playing = false;

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

    void setPos(int x, int y) {
        setPos(x, y, 0.0f);
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

    void setBuffer(AudioBuffer* buffer) {
        ALuint bufferId = buffer->getBufferId();
        alSourcei(sourceId, AL_BUFFER, bufferId);
    }

    void play() {
        if (playing) {
            return;
        }
        playing = true;
        alSourcePlay(sourceId);   
    }

    void play(AudioBuffer* buffer) {
        stop();
        setBuffer(buffer);
        play();
    }

    void stop() {
        if (!playing) {
            return;
        }
        playing = false;
        alSourceRewind(sourceId);
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