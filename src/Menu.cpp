#include "../includes/Menu.h"

Menu* Menu::instance = nullptr;

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


	//Ingame Menu
	menu["main"]["Loading"].SetTable(1, 4);
	menu["main"]["Loading"]["New Game"].SetID(0);
	menu["main"]["Loading"]["Load Game"].SetID(2);
	menu["main"]["Loading"]["Settings"].SetID(3);
	menu["main"]["Loading"]["Exit"].SetID(100);

	menu["main"]["Ingame"].SetTable(1, 5);
	menu["main"]["Ingame"]["Continue"].SetID(5);
	menu["main"]["Ingame"]["Save Game"].SetID(6);
	menu["main"]["Ingame"]["Settings"].SetID(3);
	menu["main"]["Ingame"]["Help"].SetID(4);
	menu["main"]["Ingame"]["Exit"].SetID(100);

	menu.Build();
}

bool Menu::interact(bool& isIngame, bool& stop) {
	menumanager* manager = &this->manager;
	if (isIngame) {
		manager->Open(&menu["main"]["Ingame"]);
	}
	else {
		manager->Open(&menu["main"]["Loading"]);
	}
	menuobject* command = nullptr;
	std::string sLastAction = "null";

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

	if (command != nullptr)
	{
		sLastAction = "Selected: " + command->GetName() + " ID: " + std::to_string(command->GetID());
		Level* level = &Level::getInstance();
		switch (command->GetID()) {
		case 0:
		{
			isIngame = true;
			gameConfig->operator[]("level") = 1;
			*configPath = "null";
			level->setLevel(1);
			break;
		}
		case 1:
			if (!isIngame)
				isIngame = true;
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
			if (!isIngame)
				isIngame = true;
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
		case 100:
			return false;
		default:
			std::cout << "[BUG] wrong command id\n";
			break;
		}
		manager->Reset();
	}
	

	pge->Clear(olc::CREAM);
	if (sLastAction != "null")
		AudioManager->play("MENU", sLastAction, false);
	manager->Draw(*pge, sprite, {30,30});
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