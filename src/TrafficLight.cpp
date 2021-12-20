#include "../includes/TrafficLight.h"

TrafficLightManager* TrafficLightManager::instance = nullptr;

void TrafficLightManager::Draw() {
	for (auto& trafficLight : arr) {
		trafficLight.Draw();
	}
}

int TrafficLightManager::getState(int lane) {
	return this->arr[lane].state;
}

TrafficLightManager& TrafficLightManager::getInstance() {
	if (!instance) {
		instance = new TrafficLightManager();
	}
	return *instance;
}

void TrafficLightManager::init(olc::PixelGameEngine* pge) {
	spriteList.push_back(new olc::Sprite(std::string(para::ASSETS["TRAFFIC"]["SPRITE"]["RED"])));
	spriteList.push_back(new olc::Sprite(std::string(para::ASSETS["TRAFFIC"]["SPRITE"]["YELLOW"])));
	spriteList.push_back(new olc::Sprite(std::string(para::ASSETS["TRAFFIC"]["SPRITE"]["GREEN"])));

	for (int i = 0; i < 3; i++)
		decalList.push_back(new olc::Decal(spriteList[i]));
	TrafficLight lane3(pge, { 0, 0.71f }, 7, 2, 6, spriteList, decalList);
	TrafficLight lane4(pge, { float(spriteList[0]->width), 0.56f }, 10, 10, 10, spriteList, decalList);
	arr.push_back(lane3);
	arr.push_back(lane4);
}

void TrafficLight::Draw() {
	int time = double(timeToChangeState - clock()) / CLOCKS_PER_SEC;
	if (time < 0) {
		state = (state + 1) % 3;
		timeToChangeState = clock() + duration[state] * 1000;
	}
	if (this->position.x != 0) {
		olc::vf2d pos = { float(pge->ScreenWidth()) - float(this->position.x * 0.015f), this->position.y * pge->ScreenHeight() };
		pge->DrawDecal( pos, decalList[state], {0.015f, 0.015f});
	}
	else
		pge->DrawDecal(olc::vf2d({ this->position.x * pge->ScreenWidth(), this->position.y * pge->ScreenHeight() }), decalList[state], {0.015f, 0.015f});
}

void TrafficLight::setDuration(double red, double yellow, double green) {
	duration[0] = red;
	duration[1] = yellow;
	duration[2] = green;
}

TrafficLight::TrafficLight(olc::PixelGameEngine* pge, olc::vf2d position, double red, double yellow, double green, std::vector<olc::Sprite*>& spriteList, std::vector<olc::Decal*>& decalList) {
	state = 2;
	this->pge = pge;
	this->spriteList = spriteList;
	this->decalList = decalList;
	this->position = position;
	timeToChangeState = clock() + green * 1000;
	duration[0] = red;
	duration[1] = yellow;
	duration[2] = green;
}