#include "../includes/CPeople.h"

CPeople::CPeople(olc::PixelGameEngine* pge) : pge(pge), Collider2D() {
	speed = 50;
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
	position.y -= speed * fElapsedTime;
	velocity.y = -speed;
	position.y = std::max(float(0), position.y);
}

void CPeople::Left(float fElapsedTime) {
	//std::cout << "Moving Left\n";
	position.x -= speed * fElapsedTime;
	velocity.x = -speed;
	position.x = std::max(float(0), position.x);
}

void CPeople::Right(float fElapsedTime) {
	//std::cout << "Moving Right\n";
	position.x += speed * fElapsedTime;
	velocity.x = speed;
	position.x = std::min(float(pge->ScreenWidth() - 20), position.x);
}
void CPeople::Down(float fElapsedTime) {
	//std::cout << "Moving Down\n";
	position.y += speed * fElapsedTime;
	velocity.y = speed;
	position.y = std::min(float(pge->ScreenHeight() - 30), position.y);
}

// template<class T>   // t không thể dùng template  // tìm cách khắc phục giúp t
bool CPeople::isImpact(CAnimal* object, float fElapsedTime)
{
	//Check if standing in safe area
	olc::vf2d _size = size();
	float pos = position.y + _size.y;
	if (pos > pge->ScreenHeight() * 0.85 || pos < pge->ScreenHeight() * 0.2)
		return false;
	if (pos < pge->ScreenHeight() * 0.55 && pos > pge->ScreenHeight() * 0.5)
		return false;


	std::vector<rect> vRects;   // vector chứa các bounding box của objects // {pos, size}
	vRects.push_back({ getPosition(), size() });   // vRects[0] là CPeople

	vRects[0].vel = getVelocity();
	//vRects[0].vel.x = 100;
	//vRects[0].vel.y = 100;

	vRects.push_back({ object->getPosition(), object->size() }); // các vRects tiếp theo là các objects càn kiểm tra va chạm

	olc::vf2d cp, cn;
	float t = 0;
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (RectVsRect(&vRects[0], &vRects[i]))
		{
			isdead = true;
			return true;
		}
		//if (ResolveDynamicRectVsRect(&vRects[0], fElapsedTime, &vRects[i]))
		//{
		//	isdead = true;
		//	return true;
		//}
		//if (DynamicRectVsRect(&vRects[0], fElapsedTime, vRects[i], cp, cn, t))  // kiểm tra va chạm
		//{
		//	isdead = true;
		//	return true;
		//}
	}
	return false;
}

bool CPeople::isImpact(CVehicle* object, float fElapsedTime)
{
	//Check if standing in safe area
	olc::vf2d _size = size();
	float pos = position.y + _size.y;
	if (pos >= pge->ScreenHeight() * 0.85 || pos < pge->ScreenHeight() * 0.2)
		return false;
	if (pos < pge->ScreenHeight() * 0.55 && pos > pge->ScreenHeight() * 0.5)
		return false;


	std::vector<rect> vRects;
	vRects.push_back({ getPosition(), size() });
	vRects[0].vel = getVelocity();

	vRects.push_back({ object->getPosition(), object->size() });   // Nên đưa tất cả objects vào đây

	olc::vf2d cp, cn;
	float t = 0;
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (DynamicRectVsRect(&vRects[0], fElapsedTime, vRects[i], cp, cn, t))
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
