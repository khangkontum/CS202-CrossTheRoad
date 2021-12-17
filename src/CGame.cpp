#include "../includes/CGame.h"

CGame::CGame() {
	sAppName = "Road Crossing";
}

CGame::~CGame()
{
	AudioManager->DestroyAudio();
}

bool CGame::OnUserCreate() {
	if (!olc::SOUND::InitialiseAudio()) {
		std::cerr << "Cannot init audio" << std::endl;
		return false;
	}
	std::cout << "Press H for instruction.\n";

	AudioManager->mute(false);   // Tắt tất cả âm thanh
	AudioManager->stopBackground(false); // Tắt âm thanh background

	int option;
	resetState = true;
	configPath = "null";

	/*
	bool ok = true;
	do {

		std::cout << "1. New game.\n";
		std::cout << "2. Load game.\n";
		std::cout << "3. Settings.\n";
		std::cout << "Input: ";
		std::cin >> option;
		ok = false;
		switch (option) {
		case 1:
		{
			gameConfig["level"] = 1;
			configPath = "null";
			break;
		}
		case 2:
		{
			std::cout << "Path to saved game: ";
			std::cin >> configPath;
			std::ifstream fi(configPath);
			if (!fi) {
				std::cout << "File does not exsist. Please try again.\n";
				ok = true;
				break;
			}
			fi >> gameConfig;
			fi.close();
			break;
		}
		default:
			std::cout << "Wrong option. Please choose again.\n";
			ok = true;
			break;
		}
	} while (ok);
	*/


	stop = false;
	isIngame = false;
	menu = &Menu::getInstance();
	menu->init(this, &gameConfig, &configPath);
	cPeople = std::make_unique<CPeople>(this);
	background = std::make_unique<Background>(this);

	level = &Level::getInstance();
	level->setDefaultGap(cPeople->size().x);


	//loading 
	loadingDefault();

	return true;
}

bool CGame::OnUserUpdate(float fElapsedTime) {
	if (!isIngame) {
		return menu->interact(isIngame, stop);
	}
	if (resetState) {
		std::cout << "** Level " << level->currentLevel() << " **\n" << '\n';
		resetState = false;
	}

	//Pause
	if (GetKey(olc::Key::ESCAPE).bPressed) {
		if (stop)
			std::cout << "Unpaused\n";
		else
			std::cout << "Paused\n";
		stop = true;
	}
	if (stop) {
		return menu->interact(isIngame, stop);
	}

	//Passed Level
	if (GetKey(olc::Key::ENTER).bPressed && cPeople.get()->isFinish()) {
		cPeople.get()->reset();
		level->levelUp();
		resetState = true;
	}

	//Reset
	if (GetKey(olc::Key::R).bPressed) {
		std::cout << "Restart\n";
		cPeople.get()->reset();
		resetState = true;
	}

	//Save game
	if (GetKey(olc::Key::L).bPressed) {
		while (configPath == "null") {
			std::cout << "Path to save game: ";
			std::cin >> configPath;
		}
		std::ofstream fo(configPath);
		gameConfig["level"] = level->currentLevel();
		fo << gameConfig.dump();
		fo.close();
		std::cout << "Saved\n";
	}

	bool DEBUG = false;  // DEBUG COLLISION

	if (!cPeople.get()->isDead()) {
		if (DEBUG) Clear(olc::CREAM);

		//Handle user input
		if (GetKey(olc::Key::W).bHeld || GetKey(olc::Key::UP).bHeld)
			cPeople.get()->Up(fElapsedTime);
		if (GetKey(olc::Key::S).bHeld || GetKey(olc::Key::DOWN).bHeld)
			cPeople.get()->Down(fElapsedTime);
		if (GetKey(olc::Key::A).bHeld || GetKey(olc::Key::LEFT).bHeld)
			cPeople.get()->Left(fElapsedTime);
		if (GetKey(olc::Key::D).bHeld || GetKey(olc::Key::RIGHT).bHeld)
			cPeople.get()->Right(fElapsedTime);

		if (DEBUG) DrawRect(cPeople->getPosition(), cPeople->size(), olc::BLUE);

		//Move object
		birdSpawner.get()->move(fElapsedTime);
		dinosaurSpawner.get()->move(fElapsedTime);
		truckSpawner.get()->move(fElapsedTime);
		carSpawner.get()->move(fElapsedTime);

		//Spawn object
		birdSpawner.get()->spawn(fElapsedTime);
		dinosaurSpawner.get()->spawn(fElapsedTime);
		truckSpawner.get()->spawn(fElapsedTime);
		carSpawner.get()->spawn(fElapsedTime);

		// Kiểm tra cPeople có va chạm với các object hay không
		for (auto obj : birdSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();    // nếu va chạm, in ra tên object bị va chạm
			if (DEBUG) DrawRect(obj->getPosition(), obj->size(), olc::YELLOW);
		}
		for (auto obj : dinosaurSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) DrawRect(obj->getPosition(), obj->size(), olc::GREEN);
		}
		for (auto obj : truckSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) DrawRect(obj->getPosition(), obj->size(), olc::RED);
		}
		for (auto obj : carSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) DrawRect(obj->getPosition(), obj->size(), olc::BLUE);
		}
	}
	else {
		//Draw output to save game

	}

	//Drawing
	if (!DEBUG) drawGame();



	return true;
}

void CGame::drawGame() {
	background->Draw();

	birdSpawner.get()->Draw();
	dinosaurSpawner.get()->Draw();
	carSpawner.get()->Draw();

	cPeople->Draw();

	truckSpawner.get()->Draw();
}

void CGame::loadingDefault() {
	std::ifstream fi("./database/game.json");
	fi >> gameData;

	//Loading default bird
	CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * ScreenHeight() }), 1, this);
	birdSpawner = std::make_unique<ObjectSpawner<CBird*>>(bird, this);

	//Loading default dinosaur
	CDinosaur* dinosaur = new CDinosaur(olc::vf2d({ 0, float(gameData["CDinosaur"]) * ScreenHeight() }), -1, this);
	dinosaurSpawner = std::make_unique<ObjectSpawner<CDinosaur*>>(dinosaur, this);

	//Loading default car
	CCar* car = new CCar(olc::vf2d({ 0, float(gameData["CCar"]) * ScreenHeight() }), -1, this);
	carSpawner = std::make_unique<ObjectSpawner<CCar*>>(car, this);

	//Loading default truck
	CTruck* truck = new CTruck(olc::vf2d({ 0, float(gameData["CTruck"]) * ScreenHeight() }), 1, this);
	truckSpawner = std::make_unique<ObjectSpawner<CTruck*>>(truck, this);
}