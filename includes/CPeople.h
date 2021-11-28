#pragma once

#include "olcPixelGameEngine.h"
#include "CObstacle.h"

class CPeople : public olc::PixelGameEngine {
public:
	CPeople();

	void Draw(olc::PixelGameEngine* pge);
	void Up(float fElapsedTime);
	void Left(float fElapsedTime);
	void Right(float fElapsedTime);
	void Down(float fElapsedTime);
	bool isImpact(const CObstacle*& object);
	bool isFinish();
	bool isDead();

private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> decal;
	float speed;
	olc::vf2d position;
};

/*
class CPEOPLE{
int mX, mY;
bool mState; //Trạng thái sống chết
public:
CPEOPLE();
void Up(int);
void Left(int);
void Right(int);
void Down(int);
bool isImpact(const CVEHICLE*&);
bool isImpact(const CANIMAL*&);
bool isFinish();
bool isDead();
}
*/