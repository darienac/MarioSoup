#pragma once

#include <filesystem>

#include "ResourceReader.h"

class AudioBuffer {
    private:
    static const int CHUNK_SIZE = 65536;

    std::string id;

    short audioBuffer[CHUNK_SIZE];

    ALuint buffers[2];

    int channels;
    int sampleRate;
    int samples;
    int duration;

    stb_vorbis* stream;
    bool closed = false;

    void setupBuffer(stb_vorbis* stream) {
        stb_vorbis_info info = stb_vorbis_get_info(stream);
        channels = info.channels;
        sampleRate = info.sample_rate;
        samples = stb_vorbis_stream_length_in_samples(stream) * channels;
        duration = stb_vorbis_stream_length_in_seconds(stream);
        std::printf("ID: %s, Channels: %d, Sample rate: %d\n", id.c_str(), channels, sampleRate);

        if (channels > 2) {
            throw "Too many samples, id: " + id + " (" + std::to_string(channels) + ")";
        }

        alGenBuffers(channels, buffers);

        restart();
    }

    public:
    AudioBuffer(std::string id, std::string fileName): id(id) {
        int error;
        std::string path = ResourceReader::getFullPath(ResourceReader::Audio, fileName);
        stream = stb_vorbis_open_filename(path.c_str(), &error, NULL);
        if (!stream) {
            throw "Couldn't open file";
        }

        setupBuffer(stream);
    }

    AudioBuffer(std::string id, std::filesystem::directory_entry dir): id(id) {
        int error;
        stream = stb_vorbis_open_filename(dir.path().string().c_str(), &error, NULL);
        if (!stream) {
            throw "Couldn't open file " + dir.path().string();
        }

        setupBuffer(stream);
    }

    int getNumChannels() {
        return channels;
    }

    const char* getId() {
        return id.c_str();
    }
    
    int bufferData() {
        return stb_vorbis_get_samples_short_interleaved(stream, channels, audioBuffer, CHUNK_SIZE);
    }

    void storeBufferData(ALuint bufferId, int size) {
        alBufferData(bufferId, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, audioBuffer, size, sampleRate);
    }

    ALuint* getBuffers() {
        return buffers;
    } 

    void restart() {
        stb_vorbis_seek_start(stream);

        int amount;
        amount = bufferData();
        storeBufferData(buffers[0], amount * channels * sizeof(short));
        if (channels == 2) {
            amount = bufferData();
            storeBufferData(buffers[1], amount * channels * sizeof(short));
        }
    }

    void close() {
        if (closed) {
            return;
        }
        stb_vorbis_close(stream);
        closed = true;
    }

    ~AudioBuffer() {
        close();
    }
};