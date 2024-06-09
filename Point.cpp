#include"Point.h"
#include"Matrix.h"


Point Point::operator *(Matrix& mt)
{
	Point pt;
	pt.x=mt.m[0][0]*x+mt.m[0][1]*y+mt.m[0][2]*z+mt.m[0][3]*w;
	pt.y=mt.m[1][0]*x+mt.m[1][1]*y+mt.m[1][2]*z+mt.m[1][3]*w;
	pt.w=mt.m[2][0]*x+mt.m[2][1]*y+mt.m[2][2]*z+mt.m[2][3]*w;
	return pt;
}

Point Point::operator -(Point & p2)
{
	Point pt;
	pt.x=x-p2.x;
	pt.y=y-p2.y;
	pt.z=z-p2.z;
	pt.w=1;
	return pt;
}
