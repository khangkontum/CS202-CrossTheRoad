#pragma once
#include "CVehicle.h"

class CTruck : public CVehicle {
public:
	CTruck(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CTruck* clone(olc::vf2d pos);

	void move(float fElapsedTime);
	olc::vf2d size();
	olc::vf2d getPosition();
	void Draw();
	bool isOutScreen();
	int getDirection();
};