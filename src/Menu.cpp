#include "../includes/Menu.h"

Menu* Menu::instance = nullptr;
Menu::MenuState* Menu::MenuLoading = nullptr;
Menu::MenuState* Menu::MenuSettings = nullptr;
Menu::MenuState* Menu::MenuIngame = nullptr;
Menu::MenuState* Menu::MenuCurrent = nullptr;

Menu& Menu::getInstance() {
	if (!instance)
		instance = new Menu();
	return *instance;
}

Menu::Menu() {
	gameConfig = nullptr;
	configPath = nullptr;
	pge = nullptr;
	sprite = new olc::Sprite("./assets/RetroMenu.png");

	MenuIngame = new Menu::Ingame();
	MenuLoading = new Menu::Loading();
	MenuSettings = new Menu::Settings();
	MenuCurrent = MenuLoading;

	MenuIngame->build(menu);
	MenuLoading->build(menu);
	MenuSettings->build(menu);
}
Menu::~Menu()
{
	delete MenuIngame;
	delete MenuLoading;
	delete MenuSettings;
	delete MenuCurrent;
	MenuIngame = nullptr;
	MenuLoading = nullptr;
	MenuSettings = nullptr;
	MenuCurrent = nullptr;
}

void Menu::Loading::build(menuobject& menu)
{
	menu["main"]["Loading"].SetTable(1, 4);
	menu["main"]["Loading"]["New Game"].SetID(NEWGAME);
	menu["main"]["Loading"]["Load Game"].SetID(LOADGAME);
	menu["main"]["Loading"]["Settings"].SetID(SETTING);
	menu["main"]["Loading"]["Exit"].SetID(EXIT);
	menu.Build();
}
menuobject& Menu::Loading::open(menuobject& menu)
{
	return menu["main"]["Loading"];
}
int Menu::Loading::interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level)
{
	switch (key) {
	case NEWGAME:
	{
		gameConfig->operator[]("level") = 1;
		*configPath = "null";
		level->setLevel(1);
		return START;
		break;
	}
	case LOADGAME:
	{
		if (loadGame(configPath))
		{
			level->setLevel(para::CONFIG["LEVEL"]);
			AudioManager->mute(para::CONFIG["SOUND"]["MUTE"]);
			AudioManager->stopBackground(para::CONFIG["SOUND"]["MUTESFX"]);
			std::cout << "[DEBUG] LEVEL : " << para::CONFIG["LEVEL"] << std::endl;
			return START;
		}
		break;
	}
	case SETTING:
		MenuCurrent = MenuSettings;
		break;
	case EXIT:
		return FALSE;
	default:
		std::cout << "[BUG] wrong command id\n";
		break;
	}
	return DEFAULT;
}

void Menu::Ingame::build(menuobject& menu)
{
	//Ingame Menu
	menu["main"]["Ingame"].SetTable(1, 5);
	menu["main"]["Ingame"]["Continue"].SetID(CONTINUE);
	menu["main"]["Ingame"]["Save Game"].SetID(SAVEGAME);
	menu["main"]["Ingame"]["Settings"].SetID(SETTING);
	menu["main"]["Ingame"]["Help"].SetID(HELP);
	menu["main"]["Ingame"]["Exit"].SetID(EXIT);
	menu.Build();
}
menuobject& Menu::Ingame::open(menuobject& menu)
{
	return menu["main"]["Ingame"];
}
int Menu::Ingame::interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level)
{
	switch (key) {
	case SETTING:
		MenuCurrent = MenuSettings;
		break;
	case HELP:
	{
		std::cout << "W - Up arrow    : Moving up\n";
		std::cout << "S - Down arrow  : Moving down\n";
		std::cout << "A - Left arrow  : Moving left\n";
		std::cout << "D - Right arrow : Moving right\n";
		std::cout << "P               : Pause\n";
		std::cout << "R               : Restart\n";
		std::cout << "L               : Save game\n";
		std::cout << "Enter           : Pass level\n";
		break;
	}
	case CONTINUE:
	{
		manager->Reset();
		stop = false;
		return TRUE;
	}
	case SAVEGAME:
	{
		saveGame(configPath);
		break;
	}
	case EXIT:
		return FALSE;
	default:
		std::cout << "[BUG] wrong command id\n";
		break;
	}
	return DEFAULT;
}

void Menu::Settings::build(menuobject& menu)
{
	menu["main"]["Settings"].SetTable(1, 3);
	if (AudioManager->is_Mute())
		menu["main"]["Settings"]["Unmute"].SetID(MUTE);
	else
		menu["main"]["Settings"]["Mute"].SetID(MUTE);
	if (AudioManager->is_MuteBackground())
		menu["main"]["Settings"]["On Music"].SetID(STOPMUSIC);
	else
		menu["main"]["Settings"]["Off Music"].SetID(STOPMUSIC);
	menu["main"]["Settings"]["Back"].SetID(BACK);
	menu.Build();
}
menuobject& Menu::Settings::open(menuobject& menu)
{
	//build(menu);
	return menu["main"]["Settings"];
}
int Menu::Settings::interact(int key, menumanager* manager, bool& stop, json* gameConfig, std::string* configPath, Level* level)
{
	switch (key) {
	case MUTE:
		AudioManager->mute(!AudioManager->is_Mute());
		break;
	case STOPMUSIC:
		AudioManager->stopBackground(!AudioManager->is_MuteBackground());
		break;
	case BACK:
		MenuCurrent = MenuLoading;
		break;
	default:
		std::cout << "[BUG] wrong command id\n";
		break;
	}
	return DEFAULT;
}

int Menu::interact(bool& isIngame, bool& stop) {
	menumanager* manager = &this->manager;
	manager->Open(&MenuCurrent->open(menu));

	menuobject* command = nullptr;
	std::string sLastAction = "null";
	int ID = DEFAULT;

	if (pge->GetKey(olc::Key::UP).bPressed) {
		sLastAction = "PRESS";
		manager->OnUp();
	}
	if (pge->GetKey(olc::Key::DOWN).bPressed) {
		sLastAction = "PRESS";
		manager->OnDown();
	}
	if (pge->GetKey(olc::Key::LEFT).bPressed) {
		sLastAction = "PRESS";
		manager->OnLeft();
	}
	if (pge->GetKey(olc::Key::RIGHT).bPressed) {
		sLastAction = "PRESS";
		manager->OnRight();
	}
	if (pge->GetKey(olc::Key::ENTER).bPressed) {
		sLastAction = "ENTER";
		command = manager->OnConfirm();
	}
	if (pge->GetKey(olc::Key::Z).bPressed)     manager->OnBack();
	if (pge->GetKey(olc::Key::T).bPressed)	   ID = LOADGAME, command = manager->OnConfirm();

	if (command != nullptr)
	{
		sLastAction = "Selected: " + command->GetName() + " ID: " + std::to_string(command->GetID());
		Level* level = &Level::getInstance();
		ID = ID == DEFAULT ? command->GetID() : ID;

		int res = MenuCurrent->interact(ID, manager, stop, gameConfig, configPath, level);
		if (res == FALSE)
			return false;
		else if (res == TRUE)
			return true;
		else if (res == START)
		{
			MenuCurrent = MenuIngame;
			MenuLoading = MenuIngame;
			isIngame = true;
		}
		/*switch (command->GetID()) {
		case 0:
		{
			isIngame = true;
			gameConfig->operator[]("level") = 1;
			*configPath = "null";
			level->setLevel(1);
			break;
		}
		case 1:
			if (isIngame == 0)
				isIngame = 1;
			break;
		case 2:
		{
			std::cout << "Path to game file: ";
			std::cin >> *configPath;
			std::ifstream fi(*configPath);
			if (!fi) {
				std::cout << "File does not exsist. Please try again.\n";
				break;
			}
			fi >> *gameConfig;
			fi.close();
			level->setLevel(gameConfig->operator[]("level"));
			isIngame = true;
			break;
		}
		case 3:
			MenuCurrent = MenuSettings;
			break;
		case 4:
		{
			std::cout << "W - Up arrow    : Moving up\n";
			std::cout << "S - Down arrow  : Moving down\n";
			std::cout << "A - Left arrow  : Moving left\n";
			std::cout << "D - Right arrow : Moving right\n";
			std::cout << "P               : Pause\n";
			std::cout << "R               : Restart\n";
			std::cout << "L               : Save game\n";
			std::cout << "Enter           : Pass level\n";
			break;
		}
		case 5:
		{
			manager->Reset();
			stop = false;
			return true;
		}
		case 6:
		{
			std::ofstream fo;
			if (*configPath == "null" || !fo) {
				std::cout << "Path to save game: ";
				std::cin >> *configPath;
				fo.open(*configPath);
			}
			gameConfig->operator[]("level") = level->currentLevel();
			fo << gameConfig->dump();
			fo.close();
			std::cout << "Saved\n";
			break;
		}
		case 30:
			AudioManager->mute(!AudioManager->is_Mute());
			break;
		case 31:
			AudioManager->stopBackground(!AudioManager->is_MuteBackground());
			break;
		case 101:
			isIngame = 1;
			break;
		case 100:
			return false;
		default:
			std::cout << "[BUG] wrong command id\n";
			break;
		}*/
		manager->Reset();
	}


	pge->Clear(olc::SOFT_GREEN);
	//if (sLastAction != "null")
	//	AudioManager->play("MENU", sLastAction, false);
	manager->Draw(*pge, sprite, { 30,30 });
	pge->DrawString(10, 200, sLastAction);
	return true;
}

void Menu::init(olc::PixelGameEngine* pge, json* gameConfig, std::string* configPath) {
	this->pge = pge;
	this->gameConfig = gameConfig;
	this->configPath = configPath;
}


bool Menu::execute(int command, bool& isIngame) {

	return true;
}