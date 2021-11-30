#include "../includes/CBird.h"

CBird::CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge){
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	this->speed = 10;
	sprite = std::make_unique<olc::Sprite>("./assets/bird4.png");
	decal = std::make_unique <olc::Decal>(sprite.get());
}

olc::vf2d CBird::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

void CBird::move(float fElapsedTime) {
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

void CBird::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

bool CBird::isOutScreen() {
	if (direction > 0 && position.x > pge->ScreenWidth())
		return true;
	if (direction < 0 && position.x < pge->ScreenWidth())
		return true;
	return false;
}

CBird* CBird::clone(olc::vf2d pos) {
	CBird* newBird = new CBird(pos, direction, pge);
	return newBird;
}

olc::vf2d CBird::getPosition() {
	return position;
}

int CBird::getDirection() {
	return direction;
}