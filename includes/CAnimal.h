#pragma once
#include "../lib/olcPixelGameEngine.h"

class CAnimal {
public:
	virtual void move(float fElapsedTime) = 0;
	//virtual void tell() = 0;
	virtual void Draw() = 0;
	virtual bool isOutScreen() = 0;
	virtual olc::vf2d size() = 0;
	virtual olc::vf2d getPosition() = 0;
	virtual olc::vf2d getVelocity() = 0;
	virtual int getDirection() = 0;
	virtual CAnimal* clone(olc::vf2d pos) = 0;
	virtual void getName() = 0;
	virtual int getLane() = 0;
	virtual void setPosition(olc::vf2d position) = 0;
protected:
	olc::vf2d initPosition;
	olc::vf2d position;
	olc::vf2d velocity;
	olc::PixelGameEngine* pge;
	std::shared_ptr<olc::Sprite> sprite;
	std::shared_ptr<olc::Decal> decal;
	int direction;
};