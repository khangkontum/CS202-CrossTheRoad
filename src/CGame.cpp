#include "../includes/CGame.h"

CGame::CGame() {
	sAppName = "Road Crossing";
	cPeople = new CPeople();
}

bool CGame::OnUserCreate() {
	return true;
}

bool CGame::OnUserUpdate(float fElapsedTime) {
	//Handle user input
	if (GetKey(olc::Key::W).bHeld || GetKey(olc::Key::UP).bHeld) 
		cPeople->Up(fElapsedTime);
	if (GetKey(olc::Key::S).bHeld || GetKey(olc::Key::DOWN).bHeld)
		cPeople->Down(fElapsedTime);
	if (GetKey(olc::Key::A).bHeld || GetKey(olc::Key::LEFT).bHeld)
		cPeople->Left(fElapsedTime);
	if (GetKey(olc::Key::D).bHeld || GetKey(olc::Key::RIGHT).bHeld)
		cPeople->Right(fElapsedTime);
	if (GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::Q).bHeld)
		return false;

	Clear(olc::CREAM);


	//Drawing
	cPeople->Draw(this);
	return true;
}