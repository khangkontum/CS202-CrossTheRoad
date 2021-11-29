#include "../includes/CGame.h"

CGame::CGame() {
	sAppName = "Road Crossing";
}

bool CGame::OnUserCreate() {
	cPeople = std::make_unique<CPeople>(this);
	background = std::make_unique<Background>(this);
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
	if (GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::Q).bHeld)
		return false;

	Clear(olc::CREAM);


	//Drawing
	background->Draw();
	cPeople->Draw();
	
	return true;
}