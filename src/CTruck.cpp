#include "../includes/CTruck.h"

CTruck::CTruck(olc::vf2d pos, int direction, olc::PixelGameEngine* pge) {
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	this->speed = 10;
	sprite = std::make_unique<olc::Sprite>("./assets/truck.png");
	decal = std::make_unique <olc::Decal>(sprite.get());
}

olc::vf2d CTruck::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

void CTruck::move(float fElapsedTime) {
	this->position.x += direction * speed * fElapsedTime;
	/*
	if (this->position.x > pge->ScreenWidth()) {
		this->position.x = 0;
	}

	if (this->position.x < 0) {
		this->position.x = pge->ScreenWidth();
	}
	*/
}

void CTruck::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

bool CTruck::isOutScreen() {
	if (direction > 0 && position.x > pge->ScreenWidth())
		return true;
	if (direction < 0 && position.x + sprite.get()->width * 0.1 < 0)
		return true;
	return false;
}

CTruck* CTruck::clone(olc::vf2d pos) {
	CTruck* newTruck = new CTruck(pos, direction, pge);
	return newTruck;
}

olc::vf2d CTruck::getPosition() {
	return position;
}

int CTruck::getDirection() {
	return direction;
}