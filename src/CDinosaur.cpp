#include "../includes/CDinosaur.h"

CDinosaur::CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge) {
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	sprite = std::make_unique<olc::Sprite>("./assets/dinosaur.png");
	decal = std::make_unique<olc::Decal>(sprite.get());
}

olc::vf2d CDinosaur::size() {
	float w = sprite.get()->width;
	float h = sprite.get()->height;
	return olc::vf2d({ w, h });
}

void CDinosaur::move(float fElapsedTime) {
	this->position.x += direction * fElapsedTime;
	if (this->position.x > pge->ScreenWidth()) {
		this->position.x = 0;
	}

	if (this->position.x < 0) {
		this->position.x = pge->ScreenWidth();
	}
}

void CDinosaur::Draw() {
	
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}