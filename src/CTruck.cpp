#include "../includes/CTruck.h"

CTruck::CTruck(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal) {
	this->direction = direction;
	this->pge = pge;
	this->position = pos;
	this->sprite = sprite;
	this->decal = decal;
}

CTruck::CTruck(olc::vf2d pos, int direction, olc::PixelGameEngine* pge) {
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["TRUCK"]["SPRITE"]));
	decal = std::make_unique <olc::Decal>(sprite.get());
}

olc::vf2d CTruck::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

void CTruck::move(float fElapsedTime) {
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("truck");
	this->position.x += direction * speed * fElapsedTime;
	this->velocity.x = direction * speed;
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
	CTruck* newTruck = new CTruck(pos, direction, pge, sprite, decal);
	return newTruck;
}

olc::vf2d CTruck::getPosition() {
	return position;
}

int CTruck::getDirection() {
	return direction;
}

olc::vf2d CTruck::getVelocity() {
	return velocity;
}

int CTruck::getLane() {
	return 4;
}