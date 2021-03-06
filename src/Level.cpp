#include "../includes/Level.h"
#include "string"

Level* Level::instance = nullptr;

Level::Level() {
	level = para::CONFIG["LEVEL"];
	gap = 0;
	_peopleSpeed = para::CONFIG["SPEED"]["PEOPLE"];
	_birdSpeed = para::CONFIG["SPEED"]["BIRD"];
	_dinosaurSpeed = para::CONFIG["SPEED"]["DINOSAUR"];
	_carSpeed = para::CONFIG["SPEED"]["CAR"];
	_truckSpeed = para::CONFIG["SPEED"]["TRUCK"];
}

void Level::levelUp() {
	level++;
	updateConfig();
}

int Level::currentLevel() {
	return this->level;
}

void Level::setDefaultGap(float gap) {
	this->gap = gap;
}

float Level::getGapBetweenObject() {
	return std::max(gap * 1.25, double(gap * (7 - level + 1)));
}

float Level::getSpeed(std::string obj) {
	float speed = 0;
	if (obj == "people")
		speed = _peopleSpeed;
	if (obj == "bird")
		speed = _birdSpeed;
	if (obj == "car")
		speed = _carSpeed;
	if (obj == "truck")
		speed = _truckSpeed;
	if (obj == "dinosaur")
		speed = _dinosaurSpeed;
	return std::min(speed * level * 0.5, 100.0);
}

Level& Level::getInstance() {
	if (!instance)
		instance = new Level;
	return *instance;
}

void Level::setLevel(int level) {
	this->level = level;
	updateConfig();
}

void Level::updateConfig()
{
	para::CONFIG["LEVEL"] = this->level;
}