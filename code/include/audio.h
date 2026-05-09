#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <optional> // Add this

using namespace std;
using namespace sf;

class AudioManager {
public:
    AudioManager();
    
    void playSound(const string& name);
    void playMusic(const string& path);

private:
    map<string, SoundBuffer> buffers;
    
    // SFML 3 Fix: Use optional so it doesn't need an immediate buffer
    optional<Sound> sfxPlayer; 
    Music bgMusic;

    void loadBuffer(const string& name, const string& path);
};

#endif
