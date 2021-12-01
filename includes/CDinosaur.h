#pragma once
#include "CAnimal.h"

class CDinosaur : public CAnimal {
public:
	CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CDinosaur* clone(olc::vf2d pos);

	void move(float fElapsedTime);
	//void tell();
	olc::vf2d size();
	olc::vf2d getPosition();
	void Draw();
	bool isOutScreen();
	int getDirection();
};