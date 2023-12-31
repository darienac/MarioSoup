#pragma once

#include <set>

#include "audio/AudioBuffer.h"
#include "audio/AudioSource.h"
#include "audio/MusicPlayer.h"
#include "audio/AudioCache.h"

class AudioManager {
    private:
    const ALfloat pos[3] = {0.0, 0.0, 0.0};
    const ALfloat vel[3] = {0.0, 0.0, 0.0};
    const ALfloat ori[6] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

    ALCdevice* device;
    ALCcontext* context;

    MusicPlayer* musicPlayer;
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

        musicPlayer = new MusicPlayer();
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
        musicPlayer->stopAudio();
        #endif
    }

    void playMusic(std::string path) {
        #ifndef MUTE
        musicPlayer->playAudio(path);
        #endif
    }

    void playSound(AudioBuffer* buffer, float x, float y, float z) {
        #ifndef MUTE
        AudioSource* source = new AudioSource(false, false);
        source->setPos(x, y, z);
        source->play(buffer);

        soundSources.insert(source);
        std::printf("Sound played: %s\n", buffer->getId().c_str());
        #endif
    }

    void playSound(AudioBuffer* buffer, int x, int y) {
        playSound(buffer, x, y, 0.0f);
    }

    void update() {
        #ifndef MUTE
        musicPlayer->update();
        auto it = soundSources.begin();
        while (it != soundSources.end()) {
            AudioSource* source = *it;
            // source->update();
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
        delete musicPlayer;
        for (AudioSource* source : soundSources) {
            delete source;
        }

        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
    #endif
};