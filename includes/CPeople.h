#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "CAnimal.h"

class CPeople {
public:
	CPeople(olc::PixelGameEngine* pge);

	void Draw();
	void Up(float fElapsedTime);
	void Left(float fElapsedTime);
	void Right(float fElapsedTime);
	void Down(float fElapsedTime);
	bool isImpact(const CAnimal* object);
	bool isFinish();
	bool isDead();

	olc::vf2d size();
	olc::vf2d getPosition();
	void getName() { std::cout << "CPeople" << std::endl; }
private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> decal;
	float speed;
	olc::vf2d position;
	olc::PixelGameEngine* pge;
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