#ifndef PARA_H_
#define PARA_H_
#include "../lib/json.hpp"
#include <iostream>
#include <fstream>
#include <map>

using json = nlohmann::json;

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
	para() 
	{
		std::ifstream fi("./configs/config.json");
		fi >> gameConfig;
		ASSETS = gameConfig["ASSETS"];
		CONFIG = gameConfig["CONFIG"];
	}
	~para() {};
	static para* m_instancePara;

private:
	json gameConfig;

public:
	static json ASSETS;
	static json CONFIG;
};
#endif