

#include "Point.h"

class Cube
{
	Point Vectors[8]; //��������嶥������ 
	int face[6][4];//��������顣���ÿ������ζ�����Vector������±ꡣ���ж����±갴�մ��ⲿ��ȥ��ʱ������
	int visible[6];
	float color[6][3];//6�������ɫ
public:
	Cube(float cx,float cy,float cz,float size=1);//����������ı�ƽ���������ᣬ�������������ĺ�������߳����㶥������
	void draw(int drawtype);//���������壨���߿�ʽ����䷽ʽ��
	Cube Mult(Matrix& mt);
	void CullBack(int projectiontype);//�ñ����޳���ȥ����Ȼ���ɼ���
	void drawpolygon(int v[][2],int n,float color[3],float Diffuse[3], float Specular[3]);

	void DDA(int x1,int y1,int x2,int y2,float c1[],float c2[],int scany);
	void makeET(int v[][2],int n);//�����߱�
};
