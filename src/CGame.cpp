#include "../includes/CGame.h"

CGame::CGame() {
	sAppName = "Road Crossing";
}

bool CGame::OnUserCreate() {
	cPeople = std::make_unique<CPeople>(this);
	background = std::make_unique<Background>(this);

	olc::vf2d limitSpawn = cPeople->size() * 2;

	std::ifstream fi("./database/game.json");
	fi >> gameData;

	//Loading default bird
	CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * ScreenHeight() }), 1, this);
	birdSpawner = std::make_unique<ObjectSpawner<CBird*>>(bird, limitSpawn, this);

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

	// Kiểm tra cPeople có va chạm với các object hay không

	for (auto obj : birdSpawner.get()->listObjectSpawner())
	{
		if (cPeople.get()->isImpact(obj, fElapsedTime))
			obj->getName();    // nếu va chạm, in ra tên object bị va chạm
	}
	for (auto obj : dinosaurSpawner.get()->listObjectSpawner())
	{
		if (cPeople.get()->isImpact(obj, fElapsedTime))
			obj->getName();
	}
	for (auto obj : truckSpawner.get()->listObjectSpawner())
	{
		if (cPeople.get()->isImpact(obj, fElapsedTime))
			obj->getName();
	}
	for (auto obj : carSpawner.get()->listObjectSpawner())
	{
		if (cPeople.get()->isImpact(obj, fElapsedTime))
			obj->getName();
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