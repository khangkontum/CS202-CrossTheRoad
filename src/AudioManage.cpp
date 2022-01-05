#include "../includes/AudioManager.h"
AudioManager* AudioManager::m_instanceAudioManager = nullptr;

AudioManager::AudioManager()
{
	// cần chuyển âm thanh về định dạng 16 bit, sample rate 44100

	listSound[{"PEOPLE", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["MOVE"]);
	listSound[{"PEOPLE", "IMPACT"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["IMPACT"]);
	listSound[{"PEOPLE", "DEAD"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["DEAD"]);
	listSound[{"PEOPLE", "FINSH"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["FINSH"]);

	listSound[{"CAR", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["CAR"]["SOUND"]["MOVE"]);
	listSound[{"BIRD", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["BIRD"]["SOUND"]["MOVE"]);
	listSound[{"DINOSAUR", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["DINOSAUR"]["SOUND"]["MOVE"]);
	listSound[{"TRUCK", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["TRUCK"]["SOUND"]["MOVE"]);

	listSound[{"MENU", "PRESS"}] = olc::SOUND::LoadAudioSample(para::ASSETS["MENU"]["SOUND"]["MOVE"]);
	listSound[{"MENU", "ENTER"}] = olc::SOUND::LoadAudioSample(para::ASSETS["MENU"]["SOUND"]["ENTER"]);
	listSound[{"MENU", "OPEN"}] = olc::SOUND::LoadAudioSample(para::ASSETS["MENU"]["SOUND"]["OPEN"]);

	listSound[{"BACKGROUND", ""}] = olc::SOUND::LoadAudioSample(para::ASSETS["BACKGROUND"]["SOUND"]);

	_fElapsedTime = 0;
}

void AudioManager::mute(bool _isMute)
{
	if (_isMute)
		olc::SOUND::StopAll(),
		isPlayingBackground = false;
	isMute = _isMute;
	stopBackground(_isMute);
}

void AudioManager::play(std::string name_obj, std::string action, bool isLoop)
{
	int id = listSound[{name_obj, action}].id;
	if (isMute || id == -1)
	{
		if (!isMute) std::cout << "[ERROR] CANNOT PLAY SOUND " << name_obj << " " << action << std::endl;
		return;
	}

	float lastUsed = (float)(clock() - listSound[{name_obj, action}].lastUsed) / CLOCKS_PER_SEC;

	// sau 0.2s thì mới cho phép gọi lại âm thanh
	// tránh trường hợp gọi liên tục, trùng âm thanh
	if (lastUsed > 0.2f) {
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
	if (_isMute || isMute)
	{
		stop("BACKGROUND", "");
		isPlayingBackground = false;
	}
	else if (!isPlayingBackground)
	{
		play("BACKGROUND", "", true);
		isPlayingBackground = true;
	}
	isMuteBackground = _isMute;
	updateConfig();
}

void AudioManager::updateConfig()
{
	para::CONFIG["AUDIO"]["MUTE"] = isMute;
	para::CONFIG["AUDIO"]["MUTESFX"] = isMuteBackground;
}