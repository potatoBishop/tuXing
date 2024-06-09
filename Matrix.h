#include"Vector.h"
class Point;
class Matrix
{
	
private:
	float m[4][4];//4*4����ά�任����
public:
	Matrix();
	void Identity();//���õ�ǰ����Ϊ��λ����	
	void setTranslate(float tx,float ty,float tz);//���õ�ǰ����Ϊƽ�ƾ���
	void setYRotate(float angle);//��y����ת
	void setXRotate(float angle);//��x����ת
	void setZRotate(float angle);//��z����ת
	void setCamera(Vector& eye,  Vector& center,  Vector& up);
	

	void setProjection(float d);//����ͶӰ������ƽ��͹۲����ľ���Ϊd��d=0���ʾ����ͶӰ
	Matrix operator *(Matrix & mt);//���㵱ǰ��������mt������˵Ľ����˳��Ϊ��ǰ����*mt���������Ϊ�������
	friend Point operator *(Matrix& mt,Point & p);
	friend class Point;
	friend class Cube;
};
