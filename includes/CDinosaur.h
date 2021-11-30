#pragma once
#include "CAnimal.h"

class CDinosaur : public CAnimal {
public:
	CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);

	void move(float fElapsedTime);

	olc::vf2d size();
	//void tell();
	void Draw();
};