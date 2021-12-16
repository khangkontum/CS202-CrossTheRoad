#include "../includes/AudioManager.h"
AudioManager* AudioManager::m_instanceAudioManager = nullptr;

AudioManager::AudioManager()
{
	// cần chuyển âm thanh về định dạng 16 bit, sample rate 44100

	listSound[{"PEOPLE", "MOVE"}] = olc::SOUND::LoadAudioSample("./assets/sound/sfx_step_grass_l.wav", 100);
	listSound[{"PEOPLE", "IMPACT"}] = olc::SOUND::LoadAudioSample("./assets/sound/Hitting_Wall.wav", 100);
	//listSound[{"PEOPLE", "DEAD"}] = olc::SOUND::LoadAudioSample("./assets/sound/***.wav");
	//listSound[{"PEOPLE", "FINSH"}] = olc::SOUND::LoadAudioSample("./assets/sound/***.wav");

	//listSound[{"CAR", "MOVE"}] = olc::SOUND::LoadAudioSample("./assets/sound/***.wav");
	//listSound[{"BIRD", "MOVE"}] = olc::SOUND::LoadAudioSample("./assets/sound/***.wav");
	//listSound[{"DINOSAUR", "MOVE"}] = olc::SOUND::LoadAudioSample("./assets/sound/***.wav");
	//listSound[{"TRUCK", "MOVE"}] = olc::SOUND::LoadAudioSample("./assets/sound/***.wav");

	listSound[{"BACKGROUND", ""}] = olc::SOUND::LoadAudioSample("./assets/sound/Background_Music.wav", 80);

	_fElapsedTime = 0;
}

void AudioManager::mute(bool _isMute)
{
	if (_isMute)
		olc::SOUND::StopAll();
	isMute = _isMute;
	//stopBackgound(_isMute);
}

void AudioManager::play(std::string name_obj, std::string action, bool isLoop)
{
	int id = listSound[{name_obj, action}].id;
	if (isMute || id == -1)
		return;

	float lastUsed = (float)(clock() - listSound[{name_obj, action}].lastUsed) / CLOCKS_PER_SEC;

	// sau 0.5s thì mới cho phép gọi lại âm thanh
	// tránh trường hợp gọi liên tục, trùng âm thanh
	if (lastUsed > 0.5f) {
		olc::SOUND::PlaySample(id, isLoop);
		listSound[{name_obj, action}].lastUsed = clock();
	}
}

void AudioManager::stop(std::string name_obj, std::string action)
{
	int id = listSound[{name_obj, action}].id;
	if (isMute || id == -1)
		return;
	olc::SOUND::StopSample(id);
}

void AudioManager::stopBackground(bool _isMute)
{
	if (_isMute || isMute || isMuteBackground)
	{
		stop("BACKGROUND", "");
		isPlayingBackground = false;
	}else if (!isPlayingBackground)
	{
		play("BACKGROUND", "", true);
		isPlayingBackground = true;
	}
	isMuteBackground = _isMute;
}