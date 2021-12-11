#include "../includes/CCar.h"

CCar::CCar(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal) {
	this->direction = direction;
	this->pge = pge;
	this->position = pos;
	this->sprite = sprite;
	this->decal = decal;
}

CCar::CCar(olc::vf2d pos, int direction, olc::PixelGameEngine* pge) {
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	sprite = std::make_unique<olc::Sprite>("./assets/car.png");
	decal = std::make_unique <olc::Decal>(sprite.get());
}

olc::vf2d CCar::size() {
	float w = sprite.get()->width * 0.03;
	float h = sprite.get()->height * 0.03;
	return olc::vf2d({ w, h });
}

void CCar::move(float fElapsedTime) {
	Level *level = &Level::getInstance();
	float speed = level->getSpeed("car");
	this->position.x += direction * speed * fElapsedTime;
	this->velocity.x = direction * speed;
}

void CCar::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.03f, 0.03f });
}

bool CCar::isOutScreen() {
	if (direction > 0 && position.x > pge->ScreenWidth())
		return true;
	if (direction < 0 && position.x + sprite.get()->width * 0.03 < 0)
		return true;
	return false;
}

CCar* CCar::clone(olc::vf2d pos) {
	CCar* newBird = new CCar(pos, direction, pge);
	return newBird;
}

olc::vf2d CCar::getPosition() {
	return position;
}

olc::vf2d CCar::getVelocity()
{
	return velocity;
}

int CCar::getDirection() {
	return direction;
}

int CCar::getLane() {
	return 5;
}
