#pragma once

#include <filesystem>
#include <string>

#include "ResourceReader.h"

class AudioBuffer {
    private:
    ALuint bufferId;

    std::string id;
    stb_vorbis* stream;
    stb_vorbis_info info;

    void loadBuffer(stb_vorbis* stream) {
        info = stb_vorbis_get_info(stream);
        unsigned int bufferSize = stb_vorbis_stream_length_in_samples(stream) * info.channels;
        short* buffer = new short[bufferSize];
        int amount = stb_vorbis_get_samples_short_interleaved(stream, info.channels, buffer, bufferSize);
        std::printf("Buffer loaded: (%d of %d shorts)\n", amount * info.channels, bufferSize);
        alBufferData(bufferId, (info.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, buffer, amount * info.channels * sizeof(short), info.sample_rate);

        delete[] buffer;
    }

    public:
    AudioBuffer(std::string id, std::string path): id(id) {
        int error;
        alGetError();
        alGenBuffers(1, &bufferId);
        if ((error = alGetError()) != AL_NO_ERROR) {
            std::fprintf(stderr, "AL Error: %d\n", error);
            return;
        }

        std::string fullPath = ResourceReader::getFullPath(ResourceReader::Audio, path);
        stream = stb_vorbis_open_filename(fullPath.c_str(), &error, NULL);
        if (!stream) {
            stb_vorbis_close(stream);
            std::fprintf(stderr, "vorbis error: %d file: %s\n", error, fullPath.c_str());
            throw "Couldn't open file";
        }

        loadBuffer(stream);
        stb_vorbis_close(stream);
    }

    AudioBuffer(std::string id, std::filesystem::directory_entry dir): id(id) {
        int error;
        alGetError();
        alGenBuffers(1, &bufferId);
        if ((error = alGetError()) != AL_NO_ERROR) {
            std::fprintf(stderr, "AL Error: %d\n", error);
            return;
        }

        stream = stb_vorbis_open_filename(dir.path().string().c_str(), &error, NULL);
        if (!stream) {
            stb_vorbis_close(stream);
            std::fprintf(stderr, "vorbis error: %d\n", error);
            throw "Couldn't open file";
        }

        loadBuffer(stream);
        stb_vorbis_close(stream);
    }

    std::string getId() {
        return id;
    }

    ALuint getBufferId() {
        return bufferId;
    }

    ~AudioBuffer() {
        alDeleteBuffers(1, &bufferId);
    }
};