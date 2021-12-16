#ifndef AUDIOSOURCE_H_
#define AUDIOSOURCE_H_

#include "../lib/olcPGEX_Sound.h"
#include <map>

struct audio
{
    int id = -1;
    float lastUsed = 0;
    audio& operator=(int _id) {
        id = _id;
        lastUsed = 0;
        return *this;
    }
};

typedef std::map<std::pair<std::string, std::string>, audio> PairMap;  // {"NAME OBJ", "ACTION"}

class AudioManager
{
public:
    AudioManager(AudioManager const&) = delete;
    void operator=(AudioManager const&) = delete;
    static AudioManager* getAudioManager() {
        return (!m_instanceAudioManager) ?
            m_instanceAudioManager = new AudioManager :
            m_instanceAudioManager;
    }

public:
	void mute(bool _isMute);
	void play(std::string name_obj, std::string action, bool isLoop);
    void stop(std::string name_obj, std::string action);
    void stopBackground(bool _isMute);

private:
	PairMap listSound;
    float _fElapsedTime;

private:
    AudioManager();
    ~AudioManager() {};
    static AudioManager* m_instanceAudioManager;

private:
	bool isMute = false;
    bool isMuteBackground = false;
    bool isPlayingBackground = false;
};

#endif