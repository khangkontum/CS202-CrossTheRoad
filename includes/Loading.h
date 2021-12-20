#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "para.h"

class Loading {
public:
	Loading(olc::PixelGameEngine* pge)
	{
		this->pge = pge;
		sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["BACKGROUND"]["SPRITE"]));
		grassDecal = std::make_unique<olc::Decal>(sprite.get());
	}

	void Draw()
	{
		//Draw grass
		int h = pge->ScreenHeight();
		int w = pge->ScreenWidth();
		float grassH = sprite.get()->height * 0.05f;
		float grassW = sprite.get()->width * 0.05f;
		for (float j = 0; j < w; j += grassW) {
			for (float i = float(0); i < float(h); i += grassH) {
				pge->DrawDecal(olc::vf2d({ float(j), float(i) }), grassDecal.get(), { 0.05f, 0.05f });
			}
		}
	}

private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> grassDecal;
	olc::PixelGameEngine* pge;
};