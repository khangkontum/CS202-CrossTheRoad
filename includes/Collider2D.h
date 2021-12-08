#ifndef COLLIDER2D_H_
#define COLLIDER2D_H_
#include "../lib/olcPixelGameEngine.h"

#include <algorithm>
#include <functional>
#undef min
#undef max

class Collider2D
{
public:
	Collider2D();
	~Collider2D();

	struct rect
	{
		olc::vf2d pos;
		olc::vf2d size;
		olc::vf2d vel;
	};

	//bool PointVsRect(const olc::vf2d& p, const rect* r);
	//bool RectVsRect(const rect* r1, const rect* r2);
	bool RayVsRect(const olc::vf2d& ray_origin, const olc::vf2d& ray_dir, const rect* target, olc::vf2d& contact_point, olc::vf2d& contact_normal, float& t_hit_near);
	bool DynamicRectVsRect(const rect* r_dynamic, const float fTimeStep, const rect& r_static,
		olc::vf2d& contact_point, olc::vf2d& contact_normal, float& contact_time);
	//bool ResolveDynamicRectVsRect(rect* r_dynamic, const float fTimeStep, rect* r_static);
private:
	std::array<rect*, 4> contact;
	//std::vector<rect> vRects;
};

#endif