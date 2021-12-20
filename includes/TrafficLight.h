#pragma once
#include "../lib/olcPixelGameEngine.h"
#include "para.h"

class TrafficLight {
public:
	TrafficLight(olc::PixelGameEngine* pge, olc::vf2d position, double red, double yellow, double green, std::vector<olc::Sprite*>& spriteList, std::vector<olc::Decal*>& decalList);
	void Draw();
	void setDuration(double red, double yellow, double green);

	int state;	//0: red, 1: yellow, 2: green

private:
	//TrafficLight(const TrafficLight& other);

	olc::PixelGameEngine* pge;
	std::vector<olc::Sprite*> spriteList;
	std::vector<olc::Decal*> decalList;
	olc::vf2d position;
	clock_t timeToChangeState;
	double duration[3];
};

class TrafficLightManager {
public:
	
	void Draw();
	static TrafficLightManager& getInstance();
	void init(olc::PixelGameEngine* pge);

	~TrafficLightManager() {
		for (auto& sprite : spriteList) {
			delete sprite;
		}
		for (auto& decal : decalList) {
			delete decal;
		}
	};
	
	int getState(int lane); //0: red, 1: yellow, 2: green
private:
	olc::PixelGameEngine* pge;
	std::vector<olc::Sprite*> spriteList;
	std::vector<olc::Decal*> decalList;
	static TrafficLightManager* instance;
	std::vector<TrafficLight> arr;
};

