#include "../includes/CGame.h"

CGame::CGame() {
	sAppName = "Road Crossing";
}

struct rect
{
	olc::vf2d pos;
	olc::vf2d size;
	olc::vf2d vel;

	std::array<rect*, 4> contact;
};

bool PointVsRect(const olc::vf2d& p, const rect* r)
{
	return (p.x >= r->pos.x && p.y >= r->pos.y && p.x < r->pos.x + r->size.x && p.y < r->pos.y + r->size.y);
}

bool RectVsRect(const rect* r1, const rect* r2)
{
	return (r1->pos.x < r2->pos.x + r2->size.x && r1->pos.x + r1->size.x > r2->pos.x && r1->pos.y < r2->pos.y + r2->size.y && r1->pos.y + r1->size.y > r2->pos.y);
}

bool RayVsRect(const olc::vf2d& ray_origin, const olc::vf2d& ray_dir, const rect* target, olc::vf2d& contact_point, olc::vf2d& contact_normal, float& t_hit_near)
{
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	// Cache division
	olc::vf2d invdir = 1.0f / ray_dir;

	// Calculate intersections with rectangle bounding axes
	olc::vf2d t_near = (target->pos - ray_origin) * invdir;
	olc::vf2d t_far = (target->pos + target->size - ray_origin) * invdir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	// Sort distances
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// Early rejection		
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// Closest 'time' will be the first contact
	t_hit_near = std::max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	float t_hit_far = std::min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	contact_point = ray_origin + t_hit_near * ray_dir;

	if (t_near.x > t_near.y)
		if (invdir.x < 0)
			contact_normal = { 1, 0 };
		else
			contact_normal = { -1, 0 };
	else if (t_near.x < t_near.y)
		if (invdir.y < 0)
			contact_normal = { 0, 1 };
		else
			contact_normal = { 0, -1 };

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}

bool DynamicRectVsRect(const rect* r_dynamic, const float fTimeStep, const rect& r_static,
	olc::vf2d& contact_point, olc::vf2d& contact_normal, float& contact_time)
{
	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
	if (r_dynamic->vel.x == 0 && r_dynamic->vel.y == 0)
		return false;

	// Expand target rectangle by source dimensions
	rect expanded_target;
	expanded_target.pos = r_static.pos - r_dynamic->size / 2;
	expanded_target.size = r_static.size + r_dynamic->size;

	if (RayVsRect(r_dynamic->pos + r_dynamic->size / 2, r_dynamic->vel * fTimeStep, &expanded_target, contact_point, contact_normal, contact_time))
		return (contact_time >= 0.0f && contact_time < 1.0f);
	else
		return false;
}


bool ResolveDynamicRectVsRect(rect* r_dynamic, const float fTimeStep, rect* r_static)
{
	olc::vf2d contact_point, contact_normal;
	float contact_time = 0.0f;
	if (DynamicRectVsRect(r_dynamic, fTimeStep, *r_static, contact_point, contact_normal, contact_time))
	{
		if (contact_normal.y > 0) r_dynamic->contact[0] = r_static; else nullptr;
		if (contact_normal.x < 0) r_dynamic->contact[1] = r_static; else nullptr;
		if (contact_normal.y < 0) r_dynamic->contact[2] = r_static; else nullptr;
		if (contact_normal.x > 0) r_dynamic->contact[3] = r_static; else nullptr;

		r_dynamic->vel += contact_normal * olc::vf2d(std::abs(r_dynamic->vel.x), std::abs(r_dynamic->vel.y)) * (1 - contact_time);
		return true;
	}

	return false;
}

bool CGame::OnUserCreate() {
	cPeople = std::make_unique<CPeople>(this);
	background = std::make_unique<Background>(this);

	olc::vf2d limitSpawn = cPeople->size() * 2;

	std::ifstream fi("./database/game.json");
	fi >> gameData;

	//Loading default bird
	CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * ScreenHeight()}), 1, this);
	birdSpawner = std::make_unique<ObjectSpawner<CBird*>>(bird, limitSpawn,this);
	
	//Loading default dinosaur
	CDinosaur* dinosaur = new CDinosaur(olc::vf2d({ 0, float(gameData["CDinosaur"]) * ScreenHeight() }), -1, this);
	dinosaurSpawner = std::make_unique<ObjectSpawner<CDinosaur*>>(dinosaur, limitSpawn, this);

	//Loading default car
	CCar* car = new CCar(olc::vf2d({ 0, float(gameData["CCar"]) * ScreenHeight() }), -1, this);
	carSpawner = std::make_unique<ObjectSpawner<CCar*>>(car, limitSpawn, this);

	//Loading default truck
	CTruck* truck = new CTruck(olc::vf2d({ 0, float(gameData["CTruck"]) * ScreenHeight() }), 1, this);
	truckSpawner = std::make_unique<ObjectSpawner<CTruck*>>(truck, limitSpawn, this);
	
	return true;
}

bool CGame::OnUserUpdate(float fElapsedTime) {
	//Handle user input
	if (GetKey(olc::Key::W).bHeld || GetKey(olc::Key::UP).bHeld) 
		cPeople.get()->Up(fElapsedTime);
	if (GetKey(olc::Key::S).bHeld || GetKey(olc::Key::DOWN).bHeld)
		cPeople.get()->Down(fElapsedTime);
	if (GetKey(olc::Key::A).bHeld || GetKey(olc::Key::LEFT).bHeld)
		cPeople.get()->Left(fElapsedTime);
	if (GetKey(olc::Key::D).bHeld || GetKey(olc::Key::RIGHT).bHeld)
		cPeople.get()->Right(fElapsedTime);
	if (GetKey(olc::Key::Q).bHeld)
		return false;

	birdSpawner.get()->run(fElapsedTime);
	dinosaurSpawner.get()->run(fElapsedTime);
	truckSpawner.get()->run(fElapsedTime);
	carSpawner.get()->run(fElapsedTime);

	std::vector<rect> vRects;

	vRects.push_back({ cPeople->getPosition(), cPeople->size() });

	if (GetKey(olc::Key::W).bHeld) vRects[0].vel.y = -100.0f;
	if (GetKey(olc::Key::S).bHeld) vRects[0].vel.y = +100.0f;
	if (GetKey(olc::Key::A).bHeld) vRects[0].vel.x = -100.0f;
	if (GetKey(olc::Key::D).bHeld) vRects[0].vel.x = +100.0f;

	for (auto obj : birdSpawner.get()->listObjectSpawner())
	{
		vRects.push_back({ obj->getPosition(), obj->size() });
	}
	for (auto obj : dinosaurSpawner.get()->listObjectSpawner())
	{
		vRects.push_back({ obj->getPosition(), obj->size() });
	}
	for (auto obj : truckSpawner.get()->listObjectSpawner())
	{
		vRects.push_back({ obj->getPosition(), obj->size() });
	}
	for (auto obj : carSpawner.get()->listObjectSpawner())
	{
		vRects.push_back({ obj->getPosition(), obj->size() });
	}

	olc::vf2d vMouse = { float(GetMouseX()), float(GetMouseY()) };
	olc::vf2d vPoint = { 128.0f, 120.0f };
	if (GetMouse(0).bHeld)
		vRects[0].vel += (vMouse - vRects[0].pos).norm() * 100.0f * fElapsedTime;

	// Draw all rectangles
	for (const auto& r : vRects)
		DrawRect(r.pos, r.size, olc::WHITE);

	// Sort collisions in order of distance
	olc::vf2d cp, cn;
	float t = 0, min_t = INFINITY;
	std::vector<std::pair<int, float>> z;
	//std::cout << vRects.size() << std::endl;
	// Work out collision point, add it to vector along with rect ID
	for (size_t i = 1; i < vRects.size(); i++)
	{
		if (DynamicRectVsRect(&vRects[0], fElapsedTime, vRects[i], cp, cn, t))
		{
			z.push_back({ i, t });
			std::cout << "[collision]" << i << ", " << t << std::endl;
		}
	}

	Clear(olc::CREAM);

	//Drawing
	drawGame();
	
	return true;
}

void CGame::drawGame() {
	background->Draw();

	birdSpawner.get()->Draw();
	dinosaurSpawner.get()->Draw();
	carSpawner.get()->Draw();

	cPeople->Draw();

	truckSpawner.get()->Draw();
	
}

void CGame::collider2D()
{

}
