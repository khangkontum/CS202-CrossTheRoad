#include "includes/CGame.h"

int main()
{
	CGame demo;
	if (demo.Construct(150, 200,4, 4))
		demo.Start();

	return 0;
}
