#pragma once
#include "../lib/olcPixelGameEngine.h"
#include "CPeople.h"
#include "CAnimal.h"
#include "CBird.h"
#include "CDinosaur.h"
#include "CTruck.h"
#include "CCar.h"
#include "ObjectSpawner.h"
#include "BackGround.h"
#include "../lib/json.hpp"

using json = nlohmann::json;

class CGame : public olc::PixelGameEngine {
public: 
	CGame();
	void drawGame();

	/*
	CPEOPLE getPeople();//Lấy thông tin người
	CVEHICLE * getVehicle();//Lấy danh sách các xe
	CANIMAL * getAnimal(); //Lấy danh sách các thú
	void resetGame(); // Thực hiện thiết lập lại toàn bộ dữ liệu như lúc đầu
	void exitGame(HANDLE); // Thực hiện thoát Thread
	void startGame(); // Thực hiện bắt đầu vào trò chơi
	void loadGame(istream); // Thực hiện tải lại trò chơi đã lưu
	void saveGame(istream); // Thực hiện lưu lại dữ liệu trò chơi
	void pauseGame(HANDLE); // Tạm dừng Thread
	void resumeGame(HANDLE); //Quay lai Thread
	void updatePosPeople(char); //Thực hiện điều khiển di chuyển của CPEOPLE
	void updatePosVehicle(); //Thực hiện cho CTRUCK & CCAR di chuyển
	void updatePosAnimal();//Thực hiện cho CDINAUSOR & CBIRD di chuyển
	*/
	

private:
	json gameData;
	std::unique_ptr<ObjectSpawner<CBird*>> birdSpawner;
	std::unique_ptr<ObjectSpawner<CDinosaur*>> dinosaurSpawner;
	std::unique_ptr<ObjectSpawner<CCar*>> carSpawner;
	std::unique_ptr<ObjectSpawner<CTruck*>> truckSpawner;

	std::unique_ptr<CPeople> cPeople;
	std::unique_ptr<Background> background;
	/*
	CTRUCK* axt;
	CCAR * axh;
	CDINAUSOR * akl;
	CBIRD * ac;
	*/
	

public:
	bool OnUserCreate();
	bool OnUserUpdate(float fElapsedTime);
};