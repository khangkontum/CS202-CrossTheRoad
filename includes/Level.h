#pragma once
#include <iostream>
#include <algorithm>
#include "para.h"

class Level {
private:
	static Level* instance;
	int level;
	float _peopleSpeed;
	float _birdSpeed;
	float _dinosaurSpeed;
	float _carSpeed;
	float _truckSpeed;
	float gap;

	Level();

	Level(const Level& other) {}

public:
	static Level& getInstance();

	float getSpeed(std::string obj);

	float getGapBetweenObject();

	void setDefaultGap(float gap);
	
	void setLevel(int level);

	int currentLevel();

	void levelUp();

	void updateConfig();
};


