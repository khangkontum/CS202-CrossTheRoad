#include "../includes/CPeople.h"

CPeople::CPeople(olc::PixelGameEngine* pge) : pge(pge), Collider2D() {
	position = { float(pge->ScreenWidth() / 2 - 10), float(pge->ScreenHeight() - 30) };
	sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["PEOPLE"]["SPRITE"]));

	decal = std::make_unique <olc::Decal>(sprite.get());
	isdead = false;
}

void CPeople::Draw() {
	float w = sprite.get()->width * 0.1;
	float h = sprite.get()->height * 0.1;
	olc::vf2d offset = { w , h };
	if (isdead)
		pge->DrawRotatedDecal(this->position, decal.get(), 90, offset * 2, { 0.1f, 0.1f });
	else 
		pge->DrawDecal(this->position, decal.get(), { 0.1f, 0.1f });
}

void CPeople::Up(float fElapsedTime) {
	//std::cout << "Moving Up\n";
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	position.y -= speed * fElapsedTime;
	velocity.y = -speed;
	position.y = std::max(float(0), position.y);
	AudioManager->play("PEOPLE", "MOVE", false);
}

void CPeople::Left(float fElapsedTime) {
	//std::cout << "Moving Left\n";
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	position.x -= speed * fElapsedTime;
	velocity.x = -speed;
	position.x = std::max(float(0), position.x);
	AudioManager->play("PEOPLE", "MOVE", false);
}

void CPeople::Right(float fElapsedTime) {
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	//std::cout << "Moving Right\n";
	position.x += speed * fElapsedTime;
	velocity.x = speed;
	position.x = std::min(float(pge->ScreenWidth() - 20), position.x);
	AudioManager->play("PEOPLE", "MOVE", false);
}
void CPeople::Down(float fElapsedTime) {
	//std::cout << "Moving Down\n";
	Level* level = &Level::getInstance();
	float speed = level->getSpeed("people");
	position.y += speed * fElapsedTime;
	velocity.y = speed;
	position.y = std::min(float(pge->ScreenHeight() - 30), position.y);
	AudioManager->play("PEOPLE", "MOVE", false);
}

bool CPeople::isImpact(CAnimal* object, float fElapsedTime)
{
	//Check if standing in safe area
	int currentLane = getLane();
	int objLane = object->getLane();
	if (currentLane == 0 || currentLane == 3 || currentLane == 6 || abs(currentLane - objLane) > 1) // test case 1: nếu người đang ở làn thứ 4 va chạm với obj ở làn thứ 2 thì bỏ qua 
		return false;


	std::vector<rect> vRects;   // vector chứa các bounding box của objects // {pos, size}
	vRects.push_back({ getPosition(), size(), getVelocity()});   // vRects[0] là CPeople

	vRects.push_back({ object->getPosition(), object->size(), {object->getVelocity().x, -getVelocity().y} }); // các vRects tiếp theo là các objects càn kiểm tra va chạm

	//olc::vf2d cp, cn;
	//float t = 0;
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (RectVsRect(&vRects[0], &vRects[1]))  // kiểm tra va chạm
		{
			isdead = true;
			AudioManager->play("PEOPLE", "IMPACT", false);
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
	if (currentLane == 0 || currentLane == 3 || currentLane == 6 || abs(currentLane - objLane) > 1) // test case 1: nếu người đang ở làn thứ 4 va chạm với obj ở làn thứ 2 thì bỏ qua 
		return false;


	std::vector<rect> vRects;   // vector chứa các bounding box của objects // {pos, size}
	vRects.push_back({ getPosition(), size(), getVelocity() });   // vRects[0] là CPeople

	vRects.push_back({ object->getPosition(), object->size(), {object->getVelocity().x, -getVelocity().y} }); // các vRects tiếp theo là các objects càn kiểm tra va chạm

	olc::vf2d cp, cn;
	float t = 0;
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (RectVsRect(&vRects[0], &vRects[1]))  // kiểm tra va chạm
		{
			isdead = true;
			AudioManager->play("PEOPLE", "IMPACT", false);
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
	else if (pos > 0.2 && pos <= height * 0.35)
		return 1;
	//Lane 2
	else if (pos > 0.35 && pos <= height * 0.5)
		return 2;
	//Safe area
	else if (pos > height * 0.5 && pos <= height * 0.55 )
		return 3;
	//Lane 3
	else if (pos > height * 0.55 && pos <= height * 0.7)
		return 4;
	//Lane 4
	else if (pos > height * 0.7 && pos <= height * 0.85)
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