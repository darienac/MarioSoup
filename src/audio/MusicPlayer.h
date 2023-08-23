#pragma once

#include <string>

#include "ResourceReader.h"

class MusicPlayer {
    private:
    static const int CHUNK_SIZE = 65536;
    short audioBuffer[CHUNK_SIZE];

    ALuint sourceId;
    ALuint bufferIds[2];

    bool playing = false;

    stb_vorbis* stream;
    stb_vorbis_info streamInfo;

    int getBufferData() {
        return stb_vorbis_get_samples_short_interleaved(stream, streamInfo.channels, audioBuffer, CHUNK_SIZE);
    }

    void storeBufferData(ALuint bufferId, int size) {
        alBufferData(bufferId, (streamInfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, audioBuffer, size, streamInfo.sample_rate);
    }

    void restartStream() {
        stb_vorbis_seek_start(stream);

        int amount = getBufferData();
        storeBufferData(bufferIds[0], amount * sizeof(short) * streamInfo.channels);
        amount = getBufferData();
        storeBufferData(bufferIds[1], amount * sizeof(short) * streamInfo.channels);

        alSourceQueueBuffers(sourceId, 2, bufferIds);
        alSourcePlay(sourceId);
        playing = true;
    }

    public:
    MusicPlayer() {
        ALenum error;
        alGetError();
        alGenBuffers(2, bufferIds);
        if ((error = alGetError()) != AL_NO_ERROR) {
            std::fprintf(stderr, "AL Error: %d\n", error);
            return;
        }

        alGenSources(1, &sourceId);
        alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_FALSE);
    }

    void playAudio(std::string audioPath) {
        stopAudio();

        int error;
        std::string fullPath = ResourceReader::getFullPath(ResourceReader::Audio, audioPath);
        stream = stb_vorbis_open_filename(fullPath.c_str(), &error, NULL);
        if (!stream) {
            throw "Couldn't open file";
        }

        streamInfo = stb_vorbis_get_info(stream);

        restartStream();
    }

    void stopAudio() {
        if (!playing) {
            return;
        }

        alSourceRewind(sourceId);
        alSourcei(sourceId, AL_BUFFER, 0);
        stb_vorbis_close(stream);
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
            int amount = getBufferData();
            if (amount > 0) {
                std::printf("Music audio buffered: %d\n", amount);
                storeBufferData(buffer, amount * sizeof(short) * streamInfo.channels);
                alSourceQueueBuffers(sourceId, 1, &buffer);
            }
        }

        ALint state;
        alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
        
        if (state == AL_STOPPED) {
            alSourceRewind(sourceId);
            restartStream();
        }
    }

    ~MusicPlayer() {
        stopAudio();
        alDeleteSources(1, &sourceId);
        alDeleteBuffers(2, bufferIds);
    }
};