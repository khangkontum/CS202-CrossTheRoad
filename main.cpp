
#define OLC_PGE_APPLICATION
#include "./includes/CGame.h"


int main()
{
	CGame demo;
	if (demo.Construct(180, 180, 4, 4))
		demo.Start();

	return 0;
}
