#include "../includes/CPeople.h"

CPeople::CPeople(olc::PixelGameEngine* pge) : pge(pge), Collider2D() {
	position = { float(pge->ScreenWidth() / 2 - 10), float(pge->ScreenHeight() - 30) };
	sprite = std::make_unique<olc::Sprite>("./assets/grandma.png");

	decal = std::make_unique <olc::Decal>(sprite.get());
	isdead = false;
}

void CPeople::Draw() {
	pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

void CPeople::Up(float fElapsedTime) {
	//std::cout << "Moving Up\n";
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	position.y -= speed * fElapsedTime;
	velocity.y = -speed;
	position.y = std::max(float(0), position.y);
}

void CPeople::Left(float fElapsedTime) {
	//std::cout << "Moving Left\n";
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	position.x -= speed * fElapsedTime;
	velocity.x = -speed;
	position.x = std::max(float(0), position.x);
}

void CPeople::Right(float fElapsedTime) {
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	//std::cout << "Moving Right\n";
	position.x += speed * fElapsedTime;
	velocity.x = speed;
	position.x = std::min(float(pge->ScreenWidth() - 20), position.x);
}
void CPeople::Down(float fElapsedTime) {
	//std::cout << "Moving Down\n";
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	position.y += speed * fElapsedTime;
	velocity.y = speed;
	position.y = std::min(float(pge->ScreenHeight() - 30), position.y);
}

// template<class T>   // t không thể dùng template  // tìm cách khắc phục giúp t
bool CPeople::isImpact(CAnimal* object, float fElapsedTime)
{
	//Check if standing in safe area
	int currentLane = getLane();
	int objLane = object->getLane();
	if (currentLane == 0 || currentLane == 3 || currentLane == 6 || objLane != currentLane)
		return false;


	std::vector<rect> vRects;   // vector chứa các bounding box của objects // {pos, size}
	vRects.push_back({ getPosition(), size() });   // vRects[0] là CPeople

	//vRects[0].vel = getVelocity();
	//vRects[0].vel.x = 100;
	//vRects[0].vel.y = 100;

	vRects.push_back({ object->getPosition(), object->size() }); // các vRects tiếp theo là các objects càn kiểm tra va chạm
	vRects[1].vel.x = object->getVelocity().x;
	vRects[1].vel.y = -getVelocity().y;

	olc::vf2d cp, cn;
	float t = 0;
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (ResolveDynamicRectVsRect(&vRects[i], fElapsedTime, &vRects[0]))  // kiểm tra va chạm
		{
			isdead = true;
			return true;
		}
	}
	return false;
}

bool CPeople::isImpact(CVehicle* object, float fElapsedTime)
{
	//Check if standing in safe area
	int currentLane = getLane();
	int objLane = object->getLane();
	if (currentLane == 0 || currentLane == 3 || currentLane == 6 || currentLane != objLane)
		return false;


	std::vector<rect> vRects;   // vector chứa các bounding box của objects // {pos, size}
	vRects.push_back({ getPosition(), size() });   // vRects[0] là CPeople

	//vRects[0].vel = getVelocity();
	//vRects[0].vel.x = 100;
	//vRects[0].vel.y = 100;

	vRects.push_back({ object->getPosition(), object->size() }); // các vRects tiếp theo là các objects càn kiểm tra va chạm
	vRects[1].vel.x = object->getVelocity().x;
	vRects[1].vel.y = -getVelocity().y;

	olc::vf2d cp, cn;
	float t = 0;
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (ResolveDynamicRectVsRect(&vRects[i], fElapsedTime, &vRects[0]))  // kiểm tra va chạm
		{
			isdead = true;
			return true;
		}
	}
	return false;
}

bool CPeople::isFinish()
{
	olc::vf2d _size = size();
	float pos = position.y + _size.y;
	if (pos <= pge->ScreenHeight() * 0.2)
	{
		return true;
	}
	return false;
}

bool CPeople::isDead()
{
	return isdead;
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

olc::vf2d CPeople::getVelocity()
{
	return velocity;
}

void CPeople::reset() {
	position = { float(pge->ScreenWidth() / 2 - 10), float(pge->ScreenHeight() - 30) };
	isdead = false;
}

int CPeople::getLane() {
	olc::vf2d _size = size();
	float pos = position.y + _size.y;
	float height = pge->ScreenHeight();
	//Finish
	if (pos <= height * 0.2)
		return 0;
	//Lane 1
	if (pos > 0.2 && pos <= height * 0.35)
		return 1;
	//Lane 2
	if (pos > 0.35 && pos <= height * 0.5)
		return 2;
	//Safe area
	if (pos <= height * 0.55 && pos > height * 0.5)
		return 3;
	//Lane 3
	if (pos > height * 0.55 && pos <= height * 0.7)
		return 4;
	//Lane 4
	if (pos > height > 0.7 && pos <= height * 0.85)
		return 5;
	//Start
	return 6;
}

/*
* Start Gap
* h*0.85->h
*
* Road Gap:
* h*0.2 -> h*0.5 (h*0.15 lane gap)
* h*0.55->h*0.85
*
* lane1: h*0.2->h*0.35
* lane2: h*0.35->h*0.5
* lane3: h*0.55->h*0.70
* lane4: h*0.70->h*0.85
*
* End Gap
* h->h*0.2
*/