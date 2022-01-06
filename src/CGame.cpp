#include "../includes/CGame.h"

CGame::CGame(olc::PixelGameEngine* pge, int* gameState) {
	this->pge = pge;
	this->gameState = gameState;
	//sAppName = "Road Crossing";
}

CGame::~CGame()
{
	AudioManager->DestroyAudio();
}

bool CGame::OnUserCreate() {
	*gameState = *gameState + 1;
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
	isIngame = false;
	menu = &Menu::getInstance();
	menu->init(pge, &gameConfig, &configPath);
	cPeople = std::make_unique<CPeople>(pge);
	background = std::make_unique<Background>(pge);
	trafficLightManager = &TrafficLightManager::getInstance();
	trafficLightManager->init(pge);

	level = &Level::getInstance();
	level->setDefaultGap(cPeople->size().x);


	//loading 
	//loadingDefault();

	return true;
}

bool CGame::OnUserUpdate(float fElapsedTime) {
	//std::cout << trafficLightManager->getState(0) << '\n';
	//std::cout << trafficLightManager->getState(1) << '\n';
	if (!isIngame) {
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
		AudioManager->play("PEOPLE", "FINSH", false);
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
		saveGame(&configPath);
	}

	//Load game
	if (pge->GetKey(olc::Key::T).bPressed) {
		if (loadGame(&configPath))
		{
			level->setLevel(para::CONFIG["LEVEL"]);
			AudioManager->mute(para::CONFIG["SOUND"]["MUTE"]);
			AudioManager->stopBackground(para::CONFIG["SOUND"]["MUTESFX"]);
			std::cout << "Restart\n";
			cPeople.get()->reset();
			resetState = true;
		}
	}

	bool DEBUG = false;  // DEBUG COLLISION

	if (!cPeople.get()->isDead()) {
		if (DEBUG) pge->Clear(olc::CREAM);

		//Handle user input
		while (true) {
			if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld) {
				cPeople.get()->Up(fElapsedTime);
				//break;
			}
			if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld) {
				cPeople.get()->Down(fElapsedTime);
				//break;
			}
				
			if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld) {
				cPeople.get()->Left(fElapsedTime);
				//break;
			}
				
			if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) {
				cPeople.get()->Right(fElapsedTime);
				//break;
			}	
			break;
		}
		

		if (DEBUG) pge->DrawRect(cPeople->getPosition(), cPeople->size(), olc::BLUE);

		//Move object
		ObjectFactory::birdSpawner->move(fElapsedTime);
		ObjectFactory::dinosaurSpawner->move(fElapsedTime);
		ObjectFactory::truckSpawner->move(fElapsedTime);
		ObjectFactory::carSpawner->move(fElapsedTime);

		//Spawn object
		ObjectFactory::birdSpawner->spawn(fElapsedTime);
		ObjectFactory::dinosaurSpawner->spawn(fElapsedTime);
		ObjectFactory::truckSpawner->spawn(fElapsedTime);
		ObjectFactory::carSpawner->spawn(fElapsedTime);

		// Kiểm tra cPeople có va chạm với các object hay không
		for (auto obj : ObjectFactory::birdSpawner->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();    // nếu va chạm, in ra tên object bị va chạm
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::YELLOW);
		}
		for (auto obj : ObjectFactory::dinosaurSpawner->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::GREEN);
		}
		for (auto obj : ObjectFactory::truckSpawner->listObjectSpawner())
		{
			if (cPeople.get()->isImpact(obj, fElapsedTime))
				obj->getName();
			if (DEBUG) pge->DrawRect(obj->getPosition(), obj->size(), olc::RED);
		}
		for (auto obj : ObjectFactory::carSpawner->listObjectSpawner())
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

	ObjectFactory::birdSpawner->Draw();
	ObjectFactory::dinosaurSpawner->Draw();
	ObjectFactory::carSpawner->Draw();

	cPeople->Draw();

	ObjectFactory::truckSpawner->Draw();

	trafficLightManager->Draw();
}



void CGame::loadingDefault() {
	std::ifstream fi("./database/game.json");
	fi >> gameData;

	//Loading default bird
	CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * pge->ScreenHeight() }), 1, pge);
	ObjectFactory::birdSpawner = new ObjectSpawner<CBird*>(bird, pge);

	//Loading default dinosaur
	CDinosaur* dinosaur = new CDinosaur(olc::vf2d({ 0, float(gameData["CDinosaur"]) * pge->ScreenHeight() }), -1, pge);
	ObjectFactory::dinosaurSpawner = new ObjectSpawner<CDinosaur*>(dinosaur, pge);

	//Loading default car
	CCar* car = new CCar(olc::vf2d({ 0, float(gameData["CCar"]) * pge->ScreenHeight() }), -1, pge);
	ObjectFactory::carSpawner = new ObjectSpawner<CCar*>(car, pge);

	//Loading default truck
	CTruck* truck = new CTruck(olc::vf2d({ 0, float(gameData["CTruck"]) * pge->ScreenHeight() }), 1, pge);
	ObjectFactory::truckSpawner = new ObjectSpawner<CTruck*>(truck, pge);
}