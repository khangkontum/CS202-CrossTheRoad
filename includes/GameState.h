#pragma once
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_
class GameState
{
public: 
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual ~GameState() = default;
};

#endif