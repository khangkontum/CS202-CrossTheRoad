#include "../includes/para.h"
para* para::m_instancePara = nullptr;
json para::ASSETS = json();
json para::CONFIG = json();

para::para()
{
	std::ifstream fi("./configs/config.json");
	fi >> gameConfig;
	ASSETS = gameConfig["ASSETS"];
	CONFIG = gameConfig["CONFIG"];
}