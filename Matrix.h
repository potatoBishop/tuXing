#include"Vector.h"
class Point;
class Matrix
{
	
private:
	float m[4][4];//4*4的三维变换矩阵
public:
	Matrix();
	void Identity();//设置当前矩阵为单位矩阵	
	void setTranslate(float tx,float ty,float tz);//设置当前矩阵为平移矩阵
	void setYRotate(float angle);//绕y轴旋转
	void setXRotate(float angle);//绕x轴旋转
	void setZRotate(float angle);//绕z轴旋转
	void setCamera(Vector& eye,  Vector& center,  Vector& up);
	

	void setProjection(float d);//设置投影矩阵，像平面和观察中心距离为d。d=0则表示正交投影
	Matrix operator *(Matrix & mt);//计算当前矩阵对象和mt矩阵相乘的结果（顺序为当前矩阵*mt），结果仍为矩阵对象
	friend Point operator *(Matrix& mt,Point & p);
	friend class Point;
	friend class Cube;
};
