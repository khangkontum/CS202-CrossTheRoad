#pragma once
#include <iostream>

class Level {
private:
	static Level* instance;
	int level;
	float _peopleSpeed;
	float _birdSpeed;
	float _dinosaurSpeed;
	float _carSpeed;
	float _truckSpeed;

	Level() {
		level = 0;
	}

public:
	Level& getInstance() {
		if (!instance)
			instance = new Level();
		return *instance;
	}

	float peopleSpeed() {
		return _peopleSpeed * level * 0.5;
	}
};