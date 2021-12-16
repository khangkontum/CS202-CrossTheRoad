#pragma once
#ifndef CPEOPLE_H_
#define CPEOPLE_H_

#include "../lib/olcPixelGameEngine.h"
#include "Collider2D.h"
#include "CAnimal.h"
#include "Level.h"
#include "CVehicle.h"
#include "AudioManager.h"

class CPeople : public Collider2D {
public:
	CPeople(olc::PixelGameEngine* pge);

	void Draw();
	void Up(float fElapsedTime);
	void Left(float fElapsedTime);
	void Right(float fElapsedTime);
	void Down(float fElapsedTime);
	
	void reset();

	//template<class T>
	bool isImpact(CAnimal* object, float fElapsedTime);
	bool isImpact(CVehicle* object, float fElapsedTime);

	bool isFinish();
	bool isDead();
	int getLane();

	olc::vf2d size();
	olc::vf2d getPosition();
	olc::vf2d getVelocity();
	void getName() { std::cout << "CPeople" << std::endl; }
private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> decal;
	olc::vf2d position;
	olc::vf2d velocity;
	olc::PixelGameEngine* pge;
	AudioManager* AudioManager = AudioManager->getAudioManager();
	bool isdead;
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

#endif