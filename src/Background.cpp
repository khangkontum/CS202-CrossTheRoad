#include "../includes/BackGround.h"

Background::Background(olc::PixelGameEngine* pge) : pge(pge) {
	//Loading assets
	sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["BACKGROUND"]["SPRITE"]));
	grassDecal = std::make_unique<olc::Decal>(sprite.get());
}

/*
* Start Gap
* h*0.85->h
* 
* Road Gap:
* h*0.2 -> h*0.5 (h*0.15 lane gap)
* h*0.55->h*0.85
* 
* lane1: h*0.2->h*0.35
* lane2: h*0.35->h*0.5
* lane3: h*0.55->h*0.70
* lane4: h*0.70->h*0.85
* 
* End Gap
* h->h*0.2
*/
void Background::Draw() {

	//Draw grass
	int h = pge->ScreenHeight();
	int w = pge->ScreenWidth();
	float grassH = sprite.get()->height * 0.05f;
	float grassW = sprite.get()->width * 0.05f;
	for (float j = 0; j < w; j += grassW) {
		for (float i = float(0); i < float(h); i += grassH) {
			pge->DrawDecal(olc::vf2d( { float(j), float(i) }), grassDecal.get(), {0.05f, 0.05f});
		}
	}

	//Draw Road
	pge->FillRectDecal(olc::vf2d({ float(0), float(h * 0.55) }), olc::vf2d({ float(w), float(h * 0.3) }), olc::DARK_GREY);
	pge->FillRectDecal(olc::vf2d({ 0,float(h * 0.85) }), olc::vf2d({ float(w), float(1) })); //Draw outliner
	DrawBreakLine(0, h * 0.7, w, h * 0.7);
	pge->FillRectDecal(olc::vf2d({ 0,float(h * 0.55) }), olc::vf2d({ float(w), float(1) }));

	//Draw Road
	pge->FillRectDecal(olc::vf2d({ float(0), float(h * 0.2) }), olc::vf2d({ float(w), float(h * 0.3) }), olc::DARK_GREY);
	DrawBreakLine(0, h * 0.2 + (h * 0.3 / 2), w, h * 0.2 + (h * 0.3 / 2));
	pge->FillRectDecal(olc::vf2d({ 0,float(h * 0.5) }), olc::vf2d({ float(w), float(1) }));

	//Draw Victory Line
	pge->FillRectDecal(olc::vf2d({ 0,float(h * 0.2 - 1) }), olc::vf2d({ float(w), float(1) }));
	for (float j = 0; j < w; j += grassW) {
		for (float i = 0; i < float(h * 0.2 - 1 - grassH); i += grassH) {
			pge->DrawDecal(olc::vf2d{ float(j), float(i ) }, grassDecal.get(), { 0.05f, 0.05f });
		}
	}

}

void Background::DrawLoading(int loaded, int total) {
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

	//Draw Loading bar
	pge->FillRectDecal(olc::vf2d({ float(h / 2), float(0) }), olc::vf2d({ 10.0, 100.0 }));
	std::cout << loaded << ' ' << total << '\n';
}

void Background::DrawBreakLine(int x1, int y1, int x2, int y2) {
	float gap = (x1 + x2) / 10;
	for (float i = x1 + 10; i < x2; i += gap * 2) {
		pge->FillRectDecal(olc::vf2d({ i,float(y1) }), olc::vf2d({ gap, float(2) }));
	}
}