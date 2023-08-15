#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <map>

#include "audio/AudioBuffer.h"

class AudioSource {
    private:
    const ALfloat pos[3] = {0.0, 0.0, 0.0};
    const ALfloat vel[3] = {0.0, 0.0, 0.0};
    const ALfloat dir[3] = {0.0, 0.0, 1.0};

    ALuint sourceId;
    bool playing = false;
    std::map<ALuint, AudioBuffer*> buffersPlaying;

    bool loop;
    bool relative;

    public:
    AudioSource(bool loop, bool relative): loop(loop), relative(relative) {
        alGenSources(1, &sourceId);
        if (loop) {
            // alSourcei(sourceId, AL_LOOPING, AL_TRUE);
        }
        if (relative) {
            alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_TRUE);
        }

        alSourcefv(sourceId, AL_POSITION, pos);
        alSourcefv(sourceId, AL_VELOCITY, vel);
        alSourcefv(sourceId, AL_DIRECTION, dir);
    }

    void setPos(float pos[3]) {
        alSourcefv(sourceId, AL_POSITION, pos);
        std::printf("Source pos: x: %f, y: %f, z: %f\n", pos[0], pos[1], pos[2]);
    }

    void setMaxDistance(float value) {
        alSourcef(sourceId, AL_MAX_DISTANCE, value);
    }

    void setReferenceDistance(float value) {
        alSourcef(sourceId, AL_REFERENCE_DISTANCE, value);
    }

    bool isPlaying() {
        return playing;
    }

    void playBuffer(AudioBuffer& buffer) {
        int channels = buffer.getNumChannels();
        alSourceQueueBuffers(sourceId, channels, buffer.getBuffers());
        alSourcePlay(sourceId);
        buffersPlaying[buffer.getBuffers()[0]] = &buffer;
        if (channels == 2) {
            buffersPlaying[buffer.getBuffers()[1]] = &buffer;
        }
        playing = true;
    }

    void cancelBuffers() {
        alSourceStop(sourceId);
        alSourceRewind(sourceId);
        alSourcei(sourceId, AL_BUFFER, 0);
        for (auto i : buffersPlaying) {
            i.second->restart();
        }
        buffersPlaying.clear();
        playing = false;
    }

    void update() {
        if (!playing) {
            return;
        }

        ALint processed;
        ALuint buffer;
        alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &processed);
        if (processed) {
            alSourceUnqueueBuffers(sourceId, 1, &buffer);
            AudioBuffer* alBuffer = buffersPlaying[buffer];
            int channels = alBuffer->getNumChannels();
            int amount = alBuffer->bufferData();
            if (amount > 0) {
                alBuffer->storeBufferData(buffer, amount * channels * sizeof(short));
                alSourceQueueBuffers(sourceId, 1, &buffer);
            } else if (loop) {
                alBuffer->restart();
                int amount = alBuffer->bufferData();
                if (amount > 0) {
                    alBuffer->storeBufferData(buffer, amount * channels * sizeof(short));
                    alSourceQueueBuffers(sourceId, 1, &buffer);
                }
            }
        }

        ALint state;
        alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
        if (state == AL_PLAYING) {
            return;
        }
        
        cancelBuffers();
    }

    ~AudioSource() {
        alDeleteSources(1, &sourceId);
    }
};