#include "audio.h"
#include <iostream>

using namespace std;
using namespace sf;

AudioManager::AudioManager() {
    loadBuffer("hop", "../assets/sounds/hop.wav");
    loadBuffer("death", "../assets/sounds/death.wav");
}

void AudioManager::loadBuffer(const string& name, const string& path) {
    SoundBuffer buffer;
    if (buffer.loadFromFile(path))
        buffers.insert({name, move(buffer)});
}

void AudioManager::playSound(const string& name) {
    auto it = buffers.find(name);
    if (it != buffers.end()) {
        // SFML 3 Fix: Create the Sound object directly with the buffer
        sfxPlayer.emplace(it->second); 
        sfxPlayer->play();
    }
}

void AudioManager::playMusic(const string& path) {
    // SFML 3: openFromFile returns a bool/optional
    if (bgMusic.openFromFile(path)) {
        bgMusic.setLooping(true); 
        bgMusic.setVolume(20.f);
        bgMusic.play();
    }
}
