#include<math.h>
#include "Point.h"
#include "Matrix.h"
#include<iostream>


using namespace std;

const double PI=3.14;

Matrix::Matrix()
{
	Identity();
}

void Matrix::setCamera(Vector& direction, Vector& target, Vector& up) {
	Vector forward = (target - direction);
	forward.normalize();
	Vector right = forward.cross(up);
	right.normalize();
	Vector newUp = right.cross(forward);
	newUp.normalize();
	this->Identity();
	m[0][0] = right.x;
	m[0][1] = right.y;
	m[0][2] = right.z;
	m[1][0] = newUp.x;
	m[1][1] = newUp.y;
	m[1][2] = newUp.z;
	m[2][0] = -forward.x;
	m[2][1] = -forward.y;
	m[2][2] = -forward.z;
	m[0][3] = -right.dot(direction);
	m[1][3] = -newUp.dot(direction);
	m[2][3] = forward.dot(direction);
}

//��λ���任���󣬽�������������Ϊ��λ����
void Matrix::Identity()
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			m[i][j]=(i==j);
}
Matrix Matrix::operator *(Matrix & mt)//m*mt
{
	Matrix mr;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			mr.m[i][j]=0;
			for(int k=0;k<4;k++)  mr.m[i][j]+=(m[i][k]*mt.m[k][j]);
		}
	return mr;
}
void Matrix::setTranslate(float tx,float ty,float tz)
{
	this->Identity();
	m[0][3]=tx; m[1][3]=ty; m[2][3]=tz;
}

void Matrix::setYRotate(float angle)//��y����ת
{
	this->Identity();
	m[0][0]=cos(angle/180.0*PI); m[0][2]=-sin(angle/180.0*PI);
	m[2][0]=sin(angle/180.0*PI); m[2][2]=cos(angle/180.0*PI);
}

void Matrix::setXRotate(float angle)//��x����ת
{
	this->Identity();
	m[1][2]=sin(angle/180.0*PI); m[1][1]=cos(angle/180.0*PI);
	m[2][2]=cos(angle/180.0*PI); m[2][1]=-sin(angle/180.0*PI);
}

void Matrix::setZRotate(float angle)//��z����ת
{
	this->Identity();
	m[1][0]=-sin(angle/180.0*PI); m[0][0]=cos(angle/180.0*PI);
	m[1][1]=cos(angle/180.0*PI); m[0][1]=sin(angle/180.0*PI);
}
void Matrix::setProjection(float d)//���ݽ���d����ͶӰ���󡣵�d=0ʱ����ʾ����ͶӰ
{
	this->Identity();
	if(fabs(d)<1e-5) {m[2][2]=0;return;}
	m[3][3]=m[2][2]=0;
	m[3][2]=1/d;
}


Point operator *(Matrix& mt,Point & p)
{
	Point pt;
	pt.x=mt.m[0][0]*p.x+mt.m[0][1]*p.y+mt.m[0][2]*p.z+mt.m[0][3]*p.w;
	pt.y=mt.m[1][0]*p.x+mt.m[1][1]*p.y+mt.m[1][2]*p.z+mt.m[1][3]*p.w;
	pt.z=mt.m[2][0]*p.x+mt.m[2][1]*p.y+mt.m[2][2]*p.z+mt.m[2][3]*p.w;
	pt.w=mt.m[3][0]*p.x+mt.m[3][1]*p.y+mt.m[3][2]*p.z+mt.m[3][3]*p.w;
	
	cout << " �� " <<pt.x <<" " << pt.y <<" " << pt.z << " " << pt.w << endl;
	return pt;
}
