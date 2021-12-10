#pragma once
#include "CAnimal.h"

class CDinosaur : public CAnimal {
public:
	CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal);

	CDinosaur* clone(olc::vf2d pos);

	void move(float fElapsedTime);
	//void tell();
	olc::vf2d size();
	olc::vf2d getPosition();
	olc::vf2d getVelocity();
	void Draw();
	bool isOutScreen();
	int getDirection();
	void getName() { std::cout << "CDinosaur" << std::endl; }
};