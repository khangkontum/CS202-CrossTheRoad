#include "../includes/CGame.h"

CGame::CGame(olc::PixelGameEngine* pge) {
	this->pge = pge;
	//sAppName = "Road Crossing";
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

	stop = false;
	isIngame = 0;
	menu = &Menu::getInstance();
	menu->init(pge, &gameConfig, &configPath);
	cPeople = std::make_unique<CPeople>(pge);
	background = std::make_unique<Background>(pge);
	trafficLightManager = &TrafficLightManager::getInstance();
	trafficLightManager->init(pge);

	level = &Level::getInstance();
	level->setDefaultGap(cPeople->size().x);


	//loading 
	loadingDefault();

	return true;
}

bool CGame::OnUserUpdate(float fElapsedTime) {
	//std::cout << trafficLightManager->getState(0) << '\n';
	//std::cout << trafficLightManager->getState(1) << '\n';
	if (isIngame == 0) {
		return menu->interact(isIngame, stop);
	}
	if (resetState) {
		std::cout << "** Level " << level->currentLevel() << " **\n" << '\n';
		resetState = false;
	}

	//Pause
	if (pge->GetKey(olc::Key::ESCAPE).bPressed) {
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
	if (pge->GetKey(olc::Key::ENTER).bPressed && cPeople.get()->isFinish()) {
		cPeople.get()->reset();
		level->levelUp();
		resetState = true;
	}

	//Reset
	if (pge->GetKey(olc::Key::R).bPressed) {
		std::cout << "Restart\n";
		cPeople.get()->reset();
		resetState = true;
	}

	//Save game
	if (pge->GetKey(olc::Key::L).bPressed) {
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
		if (DEBUG) pge->Clear(olc::CREAM);

		//Handle user input
		if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld)
			cPeople.get()->Up(fElapsedTime);
		if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld)
			cPeople.get()->Down(fElapsedTime);
		if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld)
			cPeople.get()->Left(fElapsedTime);
		if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld)
			cPeople.get()->Right(fElapsedTime);

		if (DEBUG) pge->DrawRect(cPeople->getPosition(), cPeople->size(), olc::BLUE);

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
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::YELLOW);
		}
		for (auto obj : dinosaurSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::GREEN);
		}
		for (auto obj : truckSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::RED);
		}
		for (auto obj : carSpawner.get()->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::BLUE);
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

	trafficLightManager->Draw();
}

void CGame::loadingDefault() {
	std::ifstream fi("./database/game.json");
	fi >> gameData;

	//Loading default bird
	CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * pge->ScreenHeight() }), 1, pge);
	birdSpawner = std::make_unique<ObjectSpawner<CBird*>>(bird, pge);

	//Loading default dinosaur
	CDinosaur* dinosaur = new CDinosaur(olc::vf2d({ 0, float(gameData["CDinosaur"]) * pge->ScreenHeight() }), -1, pge);
	dinosaurSpawner = std::make_unique<ObjectSpawner<CDinosaur*>>(dinosaur, pge);

	//Loading default car
	CCar* car = new CCar(olc::vf2d({ 0, float(gameData["CCar"]) * pge->ScreenHeight() }), -1, pge);
	carSpawner = std::make_unique<ObjectSpawner<CCar*>>(car, pge);

	//Loading default truck
	CTruck* truck = new CTruck(olc::vf2d({ 0, float(gameData["CTruck"]) * pge->ScreenHeight() }), 1, pge);
	truckSpawner = std::make_unique<ObjectSpawner<CTruck*>>(truck, pge);
}