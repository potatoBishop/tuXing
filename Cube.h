

#include "Point.h"

class Cube
{
	Point Vectors[8]; //存放立方体顶点数组 
	int face[6][4];//多边形数组。存放每个多边形顶点在Vector数组的下标。其中顶点下标按照从外部看去逆时针排列
	int visible[6];
	float color[6][3];//6个面的颜色
public:
	Cube(float cx,float cy,float cz,float size=1);//假设立方体的边平行于坐标轴，根据立方体中心和立方体边长计算顶点坐标
	void draw(int drawtype);//绘制立方体（以线框方式或填充方式）
	Cube Mult(Matrix& mt);
	void CullBack(int projectiontype);//用背面剔除法去掉显然不可见面
	void drawpolygon(int v[][2],int n,float color[3],float Diffuse[3], float Specular[3]);

	void DDA(int x1,int y1,int x2,int y2,float c1[],float c2[],int scany);
	void makeET(int v[][2],int n);//建立边表
};
