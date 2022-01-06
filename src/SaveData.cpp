#include "../includes/SaveData.h"

bool readData(std::string configPath) {
	std::ifstream fin;
	fin.open(configPath, std::ios::binary);
	if (!fin) {
		//ofstream fout;
		//fout.open(configPath, ios::binary);
		//curentLevel = 1;
		//themeSound = 0;
		//gamePlaySound = 0;
		//fout << curentLevel << themeSound << gamePlaySound;
		//fout.close();
		return false;
	}
	int curentLevel;
	bool gamePlaySound;
	bool themeSound;
	fin >> curentLevel >> gamePlaySound >> themeSound;
	fin.close();
	para::CONFIG["LEVEL"] = curentLevel;
	para::CONFIG["SOUND"]["MUTE"] = gamePlaySound;
	para::CONFIG["SOUND"]["MUTESFX"] = themeSound;
	std::cout << "[DEBUG] GameConfigs : " << para::CONFIG["LEVEL"] << " " << para::CONFIG["SOUND"]["MUTE"] << " " << para::CONFIG["SOUND"]["MUTESFX"] << std::endl;
	return true;
}

void writeData(std::string configPath) {
	std::ofstream fout;
	fout.open(configPath, std::ios::binary | std::ios::trunc);
	fout << (int)para::CONFIG["LEVEL"] << '\n' << (bool)para::CONFIG["AUDIO"]["MUTE"] << '\n' << (bool)para::CONFIG["AUDIO"]["MUTESFX"] << '\n';
	fout.close();
}

void saveGame(std::string* configPath)
{
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_SaveDialog(NULL, NULL, &outPath);
	if (result == NFD_OKAY)
	{
		writeData(outPath);
		std::cout << "Saved\n";
	}
	else if (result == NFD_CANCEL) {
		std::cout << "User pressed cancel.\n";
	}
	else {
		std::cout << "Error: %s\n" << NFD_GetError() << std::endl;
	}

	/*
	if (*configPath == "null") {
		std::cout << "Path to save game: ";
		std::cin >> *configPath;
	}
	writeData(*configPath);
	std::cout << "Saved\n";
	*/
}

bool loadGame(std::string* configPath)
{
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

	if (result == NFD_OKAY) {

		std::cout << "Path : " << outPath << std::endl;

		if (readData(outPath))
		{
			std::cout << "Loaded\n";
			return true;
		}
		else
		{
			std::cout << "No such file or directory" << std::endl;
			*configPath = "null";
			return false;
		}
		free(outPath);
	}
	else if (result == NFD_CANCEL) {
		std::cout << "User pressed cancel.\n";
	}
	else {
		std::cout << "Error: %s\n" << NFD_GetError() << std::endl;
	}
	return false;

	/*if (*configPath == "null") {
		std::cout << "Path to load game: ";
		std::cin >> *configPath;
	}
	if (readData(*configPath))
	{
		std::cout << "Loaded\n";
		return true;
	}
	else
	{
		std::cout << "No such file or directory" << std::endl;
		*configPath = "null";
		return false;
	}*/
}