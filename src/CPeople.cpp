#include "../includes/CPeople.h"

CPeople::CPeople() {
	speed = 20;
	position = {float( 150 / 2), float(150)};
}

void CPeople::Draw(olc::PixelGameEngine* pge) {
	sprite = std::make_unique<olc::Sprite>("./assets/grandma.png");
	decal = std::make_unique <olc::Decal>(sprite.get());
	pge->DrawDecal(this->position, decal.get(), {0.1f, 0.1f});
}

void CPeople::Up(float fElapsedTime) {
	std::cout << "Moving Up\n";
	position.y -= speed * fElapsedTime;
}
void CPeople::Left(float fElapsedTime) {
	std::cout << "Moving Left\n";
	position.x += speed * fElapsedTime;
}
void CPeople::Right(float fElapsedTime) {
	std::cout << "Moving Right\n";
	position.x -= speed * fElapsedTime;
}
void CPeople::Down(float fElapsedTime) {
	std::cout << "Moving Down\n";
	position.y += speed * fElapsedTime;
}
bool isImpact(const CObstacle*& object) {
	return true;
}
bool isFinish() {
	return true;
}
bool isDead() {
	return true;
}