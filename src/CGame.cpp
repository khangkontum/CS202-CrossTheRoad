#include "../includes/CGame.h"

CGame::CGame() {
	sAppName = "Road Crossing";
}

bool CGame::OnUserCreate() {
	cPeople = std::make_unique<CPeople>(this);
	background = std::make_unique<Background>(this);

	olc::vf2d limitSpawn = cPeople->size() * 2;
	

	std::ifstream fi("./database/game.json");
	fi >> gameData;

	std::cout << ( float(gameData["CBird"]) * ScreenHeight() ) << '\n';

	//Loading default bird
	CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * ScreenHeight()}), 1, this);
	birdSpawner = std::make_unique<ObjectSpawner<CBird*>>(bird, limitSpawn,this);
	
	
	return true;
}

bool CGame::OnUserUpdate(float fElapsedTime) {
	//Handle user input
	if (GetKey(olc::Key::W).bHeld || GetKey(olc::Key::UP).bHeld) 
		cPeople.get()->Up(fElapsedTime);
	if (GetKey(olc::Key::S).bHeld || GetKey(olc::Key::DOWN).bHeld)
		cPeople.get()->Down(fElapsedTime);
	if (GetKey(olc::Key::A).bHeld || GetKey(olc::Key::LEFT).bHeld)
		cPeople.get()->Left(fElapsedTime);
	if (GetKey(olc::Key::D).bHeld || GetKey(olc::Key::RIGHT).bHeld)
		cPeople.get()->Right(fElapsedTime);
	if (GetKey(olc::Key::Q).bHeld)
		return false;

	birdSpawner.get()->run(fElapsedTime);

	Clear(olc::CREAM);


	//Drawing
	drawGame();
	
	return true;
}

void CGame::drawGame() {
	background->Draw();
	cPeople->Draw();

	birdSpawner.get()->Draw();
}