#include "../includes/CBird.h"

CBird::CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal) {
	this->direction = direction;
	this->pge = pge;
	this->position = pos;
	this->sprite = sprite;
	this->decal = decal;
	this->initPosition = pos;
}

CBird::CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge){
	this->direction = direction;
	this->position = pos;
	this->pge = pge;
	this->initPosition = pos;
	sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["BIRD"]["SPRITE"]));
	decal = std::make_unique <olc::Decal>(sprite.get());
}

void CBird::setPosition(olc::vf2d position) {
	this->position = position;
}

olc::vf2d CBird::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

void CBird::move(float fElapsedTime) {
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("bird");
	this->position.x += direction * speed * fElapsedTime;
	this->velocity.x = direction * speed;
}

void CBird::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

bool CBird::isOutScreen() {
	if (direction > 0 && position.x > pge->ScreenWidth())
		return true;
	if (direction < 0 && position.x + sprite.get()->width * 0.1 < 0)
		return true;
	return false;
}

CBird* CBird::clone(olc::vf2d pos) {
	CBird* newBird = new CBird(pos, direction, pge, sprite, decal);
	return newBird;
}

olc::vf2d CBird::getPosition() {
	return position;
}

olc::vf2d CBird::getVelocity()
{
	return velocity;
}

int CBird::getDirection() {
	return direction;
}

int CBird::getLane() {
	return 1;
}