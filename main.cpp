#include "includes/CGame.h"
int main()
{
	Game demo;
	int32_t width = 150;  
	int32_t height = 200;
	int32_t pixel_w = 4;
	int32_t pixel_h = 4;
	bool is_full_screen = false;
	bool is_vsync = true;

	if (demo.Construct(width, height, pixel_w, pixel_h, is_full_screen, is_vsync, true))
		demo.Start();

	return 0;
}
