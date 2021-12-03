#include "../includes/CPeople.h"

CPeople::CPeople(olc::PixelGameEngine* pge) : pge(pge) {
	speed = 50;
	position = { float(pge->ScreenWidth() / 2 - 10), float(pge->ScreenHeight() - 30) };
	sprite = std::make_unique<olc::Sprite>("./assets/grandma.png");
	//std::cout << "START CPEOPLE\n";
	//collider2D = sprite.get()->SetPixel();
	//for (int x = 0; x <= sprite->height; x++)
	//{
	//	for (int y = 0; y <= sprite->width; y++)
	//	{ 
	//		olc::Pixel pixel = sprite->GetPixel(x, y);
	//		std::cout << '(' << int(pixel.r) << "," << int(pixel.g) << "," << int(pixel.b) << ')';
	//	}
	//	std::cout << std::endl;
	//}
	//system("pause");
	decal = std::make_unique <olc::Decal>(sprite.get());
}

void CPeople::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

void CPeople::Up(float fElapsedTime) {
	//std::cout << "Moving Up\n";
	position.y -= speed * fElapsedTime;
	position.y = std::max(float(0), position.y);
}

void CPeople::Left(float fElapsedTime) {
	//std::cout << "Moving Left\n";
	position.x -= speed * fElapsedTime;
	position.x = std::max(float(0), position.x);
}

void CPeople::Right(float fElapsedTime) {
	//std::cout << "Moving Right\n";
	position.x += speed * fElapsedTime;
	position.x = std::min(float(pge->ScreenWidth() - 20), position.x);
}
void CPeople::Down(float fElapsedTime) {
	//std::cout << "Moving Down\n";
	position.y += speed * fElapsedTime;
	position.y = std::min(float(pge->ScreenHeight() - 30), position.y);
}

bool isImpact(const CAnimal* object) {
	return true;
}

bool isFinish() {
	return true;
}

bool isDead() {
	return true;
}

olc::vf2d CPeople::size() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	return olc::vf2d({ w, h });
}

olc::vf2d CPeople::getPosition()
{
	return position;
}
