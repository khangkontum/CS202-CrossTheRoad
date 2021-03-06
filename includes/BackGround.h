#pragma once
#include "../lib/olcPixelGameEngine.h"
#include "para.h"

class Background {
public:
	Background(olc::PixelGameEngine* pge);
	void Draw();
	void DrawLoading(int loaded, int total);
	void DrawBreakLine(int x1, int y1, int x2, int y2);

private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> grassDecal;
	olc::PixelGameEngine* pge;
};