#include "../includes/CDinosaur.h"

CDinosaur::CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal) {
	this->direction = direction;
	this->pge = pge;
	this->position = pos;
	this->sprite = sprite;
	this->decal = decal;
	this->initPosition = pos;
}

CDinosaur::CDinosaur(olc::vf2d pos, int direction, olc::PixelGameEngine* pge) {
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	this->initPosition = pos;
	sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["DINOSAUR"]["SPRITE"]));
	decal = std::make_unique <olc::Decal>(sprite.get());
}

void CDinosaur::setPosition(olc::vf2d position) {
	this->position = position;
}

olc::vf2d CDinosaur::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

void CDinosaur::move(float fElapsedTime) {
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("dinosaur");
	this->position.x += direction * speed * fElapsedTime;
	this->velocity.x = direction * speed;
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
	CDinosaur* newDinosaur = new CDinosaur(pos, direction, pge, sprite, decal);
	return newDinosaur;
}

olc::vf2d CDinosaur::getPosition() {
	return position;
}

olc::vf2d CDinosaur::getVelocity()
{
	return velocity;
}

int CDinosaur::getDirection() {
	return direction;
}

int CDinosaur::getLane() {
	return 2;
}