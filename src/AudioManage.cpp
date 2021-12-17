#include "../includes/AudioManager.h"
AudioManager* AudioManager::m_instanceAudioManager = nullptr;

AudioManager::AudioManager()
{
	// cần chuyển âm thanh về định dạng 16 bit, sample rate 44100

	listSound[{"PEOPLE", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["MOVE"], 1);
	listSound[{"PEOPLE", "IMPACT"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["IMPACT"], 1);
	listSound[{"PEOPLE", "DEAD"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["DEAD"], 1);
	listSound[{"PEOPLE", "FINSH"}] = olc::SOUND::LoadAudioSample(para::ASSETS["PEOPLE"]["SOUND"]["FINSH"], 1);

	listSound[{"CAR", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["CAR"]["SOUND"]["MOVE"], 1);
	listSound[{"BIRD", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["BIRD"]["SOUND"]["MOVE"], 1);
	listSound[{"DINOSAUR", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["DINOSAUR"]["SOUND"]["MOVE"], 1);
	listSound[{"TRUCK", "MOVE"}] = olc::SOUND::LoadAudioSample(para::ASSETS["TRUCK"]["SOUND"]["MOVE"], 1);

	listSound[{"BACKGROUND", ""}] = olc::SOUND::LoadAudioSample(para::ASSETS["BACKGROUND"]["SOUND"], 0.2);

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
	}
	else if (!isPlayingBackground)
	{
		play("BACKGROUND", "", true);
		isPlayingBackground = true;
	}
	isMuteBackground = _isMute;
}