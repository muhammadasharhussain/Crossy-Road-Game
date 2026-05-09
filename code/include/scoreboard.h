#pragma once
#include <fstream>
#include <string>

using namespace std;

inline int loadHighScore(const string& path) {
    ifstream file(path);
    int hs = 0;
    if (file.is_open())
        file >> hs;
    return hs;
}

inline void saveHighScore(const string& path, int score) {
    ofstream file(path);
    if (file.is_open())
        file << score;
}