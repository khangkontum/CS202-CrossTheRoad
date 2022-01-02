#pragma once

#include "../lib/olcPixelGameEngine.h"

class GameState
{
public:
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual ~GameState() = default;
};
