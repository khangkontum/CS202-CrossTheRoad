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
	menu["main"].SetTable(1, 4);
	menu["main"]["New Game"].SetID(0);
	menu["main"]["Load Game"].SetID(2);
	menu["main"]["Settings"].SetID(3);
	menu["main"]["Exit"].SetID(100);

	menuIngame["main"].SetTable(1, 5);
	menuIngame["main"]["Continue"].SetID(5);
	menuIngame["main"]["Save Game"].SetID(6);
	menuIngame["main"]["Settings"].SetID(3);
	menuIngame["main"]["Help"].SetID(4);
	menuIngame["main"]["Exit"].SetID(100);

	menu.Build();
	menuIngame.Build();

	manager.Open(&menu["main"]);
	managerIngame.Open(&menuIngame["main"]);

	/*
	menu["main"].SetTable(1, 4);
	menu["main"]["Attack"].SetID(101);

	menu["main"]["Magic"].SetTable(1, 2);

	menu["main"]["Magic"]["White"].SetTable(3, 6);
	auto& menuWhiteMagic = menu["main"]["Magic"]["White"];
	menuWhiteMagic["Cure"].SetID(401);
	menuWhiteMagic["Cura"].SetID(402);
	menuWhiteMagic["Curaga"].SetID(403);
	menuWhiteMagic["Esuna"].SetID(404);

	menu["main"]["Magic"]["Black"].SetTable(3, 4);
	auto& menuBlackMagic = menu["main"]["Magic"]["Black"];
	menuBlackMagic["Fire"].SetID(201);
	menuBlackMagic["Fira"].SetID(202);
	menuBlackMagic["Firaga"].SetID(203);
	menuBlackMagic["Blizzara"].SetID(205).Enable(false);
	menuBlackMagic["Blizzaga"].SetID(206).Enable(false);
	menuBlackMagic["Blizzard"].SetID(204);
	menuBlackMagic["Thunder"].SetID(207);
	menuBlackMagic["Thundara"].SetID(208);
	menuBlackMagic["Thundaga"].SetID(209);
	menuBlackMagic["Quake"].SetID(210);
	menuBlackMagic["Quake2"].SetID(211);
	menuBlackMagic["Quake3"].SetID(212);
	menuBlackMagic["Bio"].SetID(213);
	menuBlackMagic["Bio1"].SetID(214);
	menuBlackMagic["Bio2"].SetID(215);
	menuBlackMagic["Demi"].SetID(216);
	menuBlackMagic["Demi1"].SetID(217);
	menuBlackMagic["Demi2"].SetID(218);

	menu["main"]["Defend"].SetID(102);

	menu["main"]["Items"].SetTable(2, 4).Enable(false);
	menu["main"]["Items"]["Potion"].SetID(301);
	menu["main"]["Items"]["Ether"].SetID(302);
	menu["main"]["Items"]["Elixir"].SetID(303);

	//menu["main"]["Escape"].SetID(103);
	*/
}

bool Menu::interact(bool& isIngame, bool& stop) {
	menumanager* manager;
	if (isIngame) {
		manager = &this->managerIngame;
	}
	else {
		manager = &this->manager;
	}
	menuobject* command = nullptr;
	std::string sLastAction;
	if (pge->GetKey(olc::Key::M).bPressed)    manager->Open(&menu["main"]);

	if (pge->GetKey(olc::Key::UP).bPressed)    manager->OnUp();
	if (pge->GetKey(olc::Key::DOWN).bPressed)  manager->OnDown();
	if (pge->GetKey(olc::Key::LEFT).bPressed)  manager->OnLeft();
	if (pge->GetKey(olc::Key::RIGHT).bPressed) manager->OnRight();
	if (pge->GetKey(olc::Key::ENTER).bPressed) command = manager->OnConfirm();
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
	manager->Draw(*pge, sprite, {30,30});
	pge->DrawString(10, 200, sLastAction);
	return true;
}

void Menu::init(olc::PixelGameEngine* pge, json* gameConfig, std::string* configPath) {
	this->pge = pge;
	this->gameConfig = gameConfig;
	this->configPath = configPath;
}

void Menu::draw() {
	manager.Draw(*pge, sprite, {30, 30});
}

bool Menu::execute(int command, bool& isIngame) {
	
	return true;
}