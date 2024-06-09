
#ifndef __POINT_H__

#define __POINT_H__

class Matrix;
//自定义三维点
class Point
{
public:	
	float x;
	float y;
	float z;
	float w;

	Point(float xx=0,float yy=0,float zz=0,float ww=1):x(xx),y(yy),z(zz),w(ww){}
	void set(float xx,float yy,float zz,float ww=1)	{x=xx;y=yy;z=zz;w=ww;}
	//Point operator *(Matrix& mt);
	Point operator *(Matrix& mt);//mt*p
	Point operator -(Point & p2);

};


#endif
