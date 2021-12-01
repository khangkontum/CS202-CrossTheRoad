#include "../includes/CDinosaur.h"

CDinosaur::CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge) {
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	this->speed = 10;
	sprite = std::make_unique<olc::Sprite>("./assets/dinosaur3.png");
	decal = std::make_unique <olc::Decal>(sprite.get());
}

olc::vf2d CDinosaur::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

void CDinosaur::move(float fElapsedTime) {
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

void CDinosaur::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

bool CDinosaur::isOutScreen() {
	if (direction > 0 && position.x > pge->ScreenWidth())
		return true;
	if (direction < 0 && position.x + sprite.get()->width * 0.1 < 0)
		return true;
	return false;
}

CDinosaur* CDinosaur::clone(olc::vf2d pos) {
	CDinosaur* newDinosaur = new CDinosaur(pos, direction, pge);
	return newDinosaur;
}

olc::vf2d CDinosaur::getPosition() {
	return position;
}

int CDinosaur::getDirection() {
	return direction;
}