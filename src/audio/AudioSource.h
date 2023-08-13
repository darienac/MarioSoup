#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <map>

#include "audio/AudioBuffer.h"

class AudioSource {
    private:
    ALuint sourceId;
    bool playing = false;
    std::map<ALuint, AudioBuffer*> buffersPlaying;

    public:
    AudioSource(bool loop, bool relative) {
        alGenSources(1, &sourceId);
        if (loop) {
            alSourcei(sourceId, AL_LOOPING, AL_TRUE);
        }
        if (relative) {
            alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_TRUE);
        }
    }

    void playBuffer(AudioBuffer& buffer) {
        alSourceQueueBuffers(sourceId, 2, buffer.getBuffers());
        alSourcePlay(sourceId);
        buffersPlaying[buffer.getBuffers()[0]] = &buffer;
        buffersPlaying[buffer.getBuffers()[1]] = &buffer;
        playing = true;
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
            int amount = alBuffer->bufferData();
            if (amount > 0) {
                alBuffer->storeBufferData(buffer, amount * 2 * sizeof(short));
                alSourceQueueBuffers(sourceId, 1, &buffer);
            }
        }

        ALint state;
        alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
        if (state == AL_PLAYING) {
            return;
        }

        for (auto i : buffersPlaying) {
            i.second->close();
        }
        buffersPlaying.clear();
        playing = false;
    }
};