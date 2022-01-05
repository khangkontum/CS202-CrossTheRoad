#ifndef PARA_H_
#define PARA_H_
#include "../lib/json.hpp"
#include <iostream>
#include <fstream>
#include <map>

using json = nlohmann::json;

enum MenuController
{
	START = 2,
	NEWGAME,
	LOADGAME,
	CONTINUE,
	SETTING,
	SAVEGAME,
	MUTE,
	STOPMUSIC,
	HELP,
	BACK,
	EXIT,
	DEFAULT
};

class para
{
public:
	para(para const&) = delete;
	void operator=(para const&) = delete;
	static para* getPara() {
		return (!m_instancePara) ?
			m_instancePara = new para :
			m_instancePara;
	}

private:
	para();
	~para() {};
	static para* m_instancePara;

private:
	json gameConfig;

public:
	static json ASSETS;
	static json CONFIG;
};
#endif