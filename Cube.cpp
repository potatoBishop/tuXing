#include <gl/glut.h>
#include <math.h>
#include"Cube.h"
#include"Matrix.h"
#include <math.h>
#include <stdio.h>
#include<iostream>
#include <gl/glut.h>
const int YDIS=250;//边表下标偏移量
#define LINE 0
#define FILL 1//drawtype=0线框,drawtype=1填充
#define  WIDTH  1000

#define  HEIGHT 1000
using namespace std;
extern Vector CameraPosition;
extern Vector TargetPosition;
extern Vector CameraDirection;
extern Vector CameraUp;
Matrix camera;

struct EDGE
{
	float ymax,xmin;
	float deltax;
	struct EDGE *next;
};

EDGE * ET[500]={0};
EDGE AELhead={0,0,0,0};
EDGE *pAEL=&AELhead;
Point eye;

float A,B,C,D;
float zb[1005][1005];
float diffuse[6][3];
float specular[6][3];
void reflect(Vector& vector, Vector& normal, Vector& result)
{
    float dotProduct = vector.dot(normal);
    result.x = vector.x - 2.0f * dotProduct * normal.x;
    result.y = vector.y - 2.0f * dotProduct * normal.y;
    result.z = vector.z - 2.0f * dotProduct * normal.z;
}

 Cube::Cube(float cx,float cy,float cz,float size)
{
	static const GLfloat ver[][3] = {// 将单位长度立方体八个顶点作坐标保存到数组
		-1.0f, -1.0f, -1.0f,      1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,      1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,      1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,      1.0f,  1.0f,  1.0f};
	static const GLint index[][4] = {// 将每个面的顶点序号保存到数组
     0, 2, 3, 1,     0, 4, 6, 2,     0, 1, 5, 4,     4, 5, 7, 6,     1, 3, 7, 5,     2, 6, 7, 3};

	 static const float colors[][3]={{1,0,0},{0,1,0},{0,0,1},{1,1,0},{0,1,1},{1,0,1}};
	for(int i=0;i<8;i++)
	{
		Vectors[i].set(cx+size*ver[i][0],cy+size*ver[i][1],cz+size*ver[i][2],1);
//		cout << Vectors[i].x <<"  " << Vectors[i].y <<" " << Vectors[i].z << endl;
	}
	for( int i=0;i<6*4;i++)
	{
		face[0][i]=index[0][i];
	}
	for(int i=0;i<6;i++)
		visible[i]=true;
	for(int i=0;i<18;i++)
		color[0][i]=colors[0][i],diffuse[0][i] = color[0][i],specular[0][i] = color[0][i];

}


//将立方体的每个顶点坐标应用变换矩阵mt
Cube Cube::Mult(Matrix& mt)
{
	Cube cb(*this);
	for(int i=0;i<8;i++)
		cb.Vectors[i]=mt*Vectors[i];
	return cb;

}

//用背面剔除法去掉显然不可见背面，将其对应的顶点索引改为-1
void Cube::CullBack(int type)//type=0正交投影，type=1透视投影
{
  //请补充代码
  if(type == 0){
//  	正交 
  	for(int i = 0; i < 6; i ++){

	  	float x1 = Vectors[face[i][1]].x - Vectors[face[i][0]].x;
	  	float y1 = Vectors[face[i][1]].y - Vectors[face[i][0]].y;
	  	float z1 = Vectors[face[i][1]].z - Vectors[face[i][0]].z;
	  	float x2 = Vectors[face[i][2]].x - Vectors[face[i][1]].x;
		float y2 = Vectors[face[i][2]].y - Vectors[face[i][1]].y;
		float z2 = Vectors[face[i][2]].z - Vectors[face[i][1]].z;
		eye = {0,0,-1};
		float z = eye.x*(y1 * z2 - z1 * y2) + eye.y*(z1 * x2 - x1 * z2) + eye.z*(x1 * y2 - y1 * x2);
			//线框图不需要光照 
		if(z <= 0){
			for(int j =  0;j < 4; j ++){
				face[i][j] = -1;
			} 				
		}
  	}
  }
  else{
  	for(int i = 0; i < 6; i ++){
	  	float x1 = Vectors[face[i][1]].x - Vectors[face[i][0]].x;
	  	float y1 = Vectors[face[i][1]].y - Vectors[face[i][0]].y;
	  	float z1 = Vectors[face[i][1]].z - Vectors[face[i][0]].z;
	  	float x2 = Vectors[face[i][2]].x - Vectors[face[i][1]].x;
		float y2 = Vectors[face[i][2]].y - Vectors[face[i][1]].y;
		float z2 = Vectors[face[i][2]].z - Vectors[face[i][1]].z;

		eye = {CameraPosition.x,CameraPosition.y,CameraPosition.z};

		eye = eye - Vectors[face[i][0]];
		float z = eye.x*(y1 * z2 - z1 * y2) + eye.y*(z1 * x2 - x1 * z2) + eye.z*(x1 * y2 - y1 * x2);

		
		if(z <= 0 ){
			for(int j =  0;j < 4; j ++){
				face[i][j] = -1;
			} 				
		}	  	
	  	
	  }
  }
}

Vector lightPosition;

// 计算点的颜色值
void calculateColor(Vector& position, Vector& normal,float _Diffuse[3], float _Specular[3])
{
    // 光源位置和颜色
    float shininess = 1000.0f;
    
    // 计算光线向量和反射向量
    Vector lightVector;
    lightVector.x = lightPosition.x - position.x;
    lightVector.y = lightPosition.y - position.y;
    lightVector.z = lightPosition.z - position.z;
    lightVector.normalize();
    
    Vector normalVector;
	//计算法向量 
    normalVector.x = normal.x;
    normalVector.y = normal.y;
	normalVector.z = normal.z;   
    normalVector.normalize();
    
    Vector reflectVector = lightPosition - normal;
    reflect(lightVector, normalVector, reflectVector);
    reflectVector.normalize();
    
    float diffuseIntensity = lightVector.dot(normalVector);
//    cout <<diffuseIntensity <<"SOSOSOSO" << endl;
    if (diffuseIntensity > 0.0f) {
        for (int i = 0; i < 3; i++) {
            _Diffuse[i] *= diffuseIntensity;
        }
    }

    float specularIntensity = pow(reflectVector.dot(lightVector), shininess);
    if (specularIntensity > 0.0f) {
        for (int i = 0; i < 3; i++) {
            _Specular[i] *= specularIntensity;
        }
    }

}

//绘制立方体（以线框方式或填充方式）
//drawtype=LINE线框,drawtype=FILL填充
void Cube::draw(int drawtype)
{
	lightPosition.setVector(0,0,-5000);
	camera.setCamera(CameraDirection,TargetPosition,CameraUp);
	for(int i =0 ; i < 1005; i ++)
		for(int j = 0; j < 1005; j ++)
			zb[i][j] = 1500;
	//绘制每个面（多边形）	
	for(int i=0;i<6;i++)
	{
//		cout << "WO" << endl;
		//如果某个面可见，就绘制该面对应的多边形,否则继续处理下一个多边形
		if(face[i][0]<0) 
			continue;
//  		glColor3f(color[i][0],color[i][1],color[i][2]);
			glColor3f(1,0,0);
		if(drawtype==LINE)//顺序连接各顶点绘制封闭线框
		{

			glBegin(GL_LINE_LOOP);//顺序连接各顶点绘制封闭边框
			for(int j=0;j<4;j++){
				float f=Vectors[face[i][j]].w;
				if(fabs(f)<1e-5) continue;//防止除零
						
				//根据齐次坐标计算点的实际坐标

				Point tmp = Vectors[face[i][j]].operator *(camera);
				float x = tmp.x / f;
				float y = tmp.y / f;
				float z = tmp.z / f;
				
				glVertex3f(x,y,z);

			}
			glEnd();	
		}
		else //FILL
		{
			//请补充代码
			int v[4][2];
			float x1 = Vectors[face[i][1]].x - Vectors[face[i][0]].x;
			float y1 = Vectors[face[i][1]].y - Vectors[face[i][0]].y;
			float z1 = Vectors[face[i][1]].z - Vectors[face[i][0]].z;
			float x2 = Vectors[face[i][2]].x - Vectors[face[i][1]].x;
			float y2 = Vectors[face[i][2]].y - Vectors[face[i][1]].y;
			float z2 = Vectors[face[i][2]].z - Vectors[face[i][1]].z;
			A = y1*z2 - z1 * y2;
			B = z1*x2 - x1 * z2;
			C = x1*y2 - x2 * y1;
			D = -(A * Vectors[face[i][0]].x + B * Vectors[face[i][0]].y + C * Vectors[face[i][0]].z);
			for (int j = 0; j < 4; j++) {
				float f = Vectors[face[i][j]].w;
				if (fabs(f) < 1e-5) continue;
				Point tmp = Vectors[face[i][j]].operator *(camera);
				v[j][0] = tmp.x / f;
				v[j][1] = tmp.y / f;
				v[j][2] = tmp.z / f;
			}
			
//			normal.x = 
			drawpolygon(v, 4, color[i],diffuse[i],specular[i]);
		}
	}	
}



//根据顶点序列v[][2]和顶点个数n建立边表
void Cube::makeET(int v[][2],int n)


{
	if(n<=0) return ;
	for(int i=0;i<n;i++)
	{             
        int x1=v[i][0],y1=v[i][1];
		int x2=v[(i+1)%n][0],y2=v[(i+1)%n][1];
		if(y1==y2)  continue;
		//计算该边的ymin,ymax,xmin, deltax,生成该边结点;
		EDGE *p=new EDGE;
		p->ymax=(y2>y1?y2:y1);
		p->xmin=(y2>y1?x1:x2);
		p->deltax=float(x2-x1)/(y2-y1);
		p->next=NULL;
		int ymin=(y2>y1?y1:y2);

		EDGE *p2=ET[ymin+YDIS];
		//将新边插入ET对应链表，保证链表有序
		if(p2==NULL)   {ET[ymin+YDIS]=p;}//直接插入表头;//链表为空则直接插入
		else
		{
			//比较当前边的xmin与ET[ymin]链表中边结点的xmin的大小关系，将当前边结点插入到合适位置ep1=当前边结点；ep2=ET[ymin]中第一个链表结点
			if(p2->xmin>p->xmin||((fabs(p->xmin-p2->xmin)<1e-5)&&(p->deltax<p2->deltax)))
			{
				p->next=ET[ymin+YDIS]; ET[ymin+YDIS]=p;
				continue;
			}
			while(p2&&p2->next&&((p->xmin>p2->next->xmin)||((fabs(p->xmin-p2->next->xmin)<1e-5)&&(p->deltax>p2->next->deltax))))
				p2=p2->next;
			if(p2==NULL) continue;//
			if(p2->next==NULL) 
			{
				p2->next=p;
				continue;
			}
			p->next=p2->next;
			p2->next=p;
		}
	}
}
int cnt = 0;

//借助ET和AEL对多边形进行扫描转换，v为二维顶点坐标序列，n为顶点个数,color为多边形颜色
void Cube::drawpolygon(int v[][2],int n,float color[3],float Diffuse[3], float Specular[3])
{
	if (C == 0.0f) return;
		makeET(v, n);
		//为了保证ET数组下标非负,计算时为y坐标增加偏移量ydis
	   //找到ET中最小非空y类;
		int scany = -251;
		for (; scany < 250; scany++) { if (ET[scany + YDIS]) break; }
		while (scany < 250)//scany为当前扫描线对应y值
		{
			//1. 如果ET[scany]类非空,将其加入AEL中
			//2. 对x取整、配对、填色
			//3. 递增scany,删除ymax==scany的边		
			if (ET[scany + YDIS] != NULL)//ET[scany]类非空,合并两条有序链表；
			{
				EDGE* p1, * p2;
				p1 = ET[scany + YDIS]; ET[scany + YDIS] = NULL;
				p2 = pAEL;
				int cnt = 0;
				while (p1 && p2 && p2->next && cnt < 10000)
				{
					if ((p1->xmin < p2->next->xmin) || ((p1->xmin == p2->next->xmin) && (p1->deltax < p2->next->deltax)))
					{
						EDGE* pt = p1->next;
						p1->next = p2->next;
						p2->next = p1;
						p1 = pt;
					}
					else if ((p1->xmin > p2->next->xmin) || ((p1->xmin == p2->next->xmin) && (p1->deltax > p2->next->deltax)))
						p2 = p2->next;
					cnt++;
				}
				if (p1 == NULL) continue;
				p2->next = p1;
			}
	
			if (pAEL && pAEL->next == NULL)
				break;//跳出while循环，结束填充算法
	
			EDGE* pael = pAEL->next;//aelhead为AEL活动边表头结点
			while (pael && pael->next)//取整、配对、填充
			{
				//如需实现深度缓冲消隐算法，需要在此处逐个像素计算、比较
	
				int x1 = pael->xmin, x2 = pael->next->xmin;
				if (pael->xmin - x1 > 1e-5) x1++;
				if (pael->next->xmin - x2 < 1e-5)x2--;
	
	
				double CurDeep = -(x1 * A + scany * B + D) / C;
				double DeepStep = -A / C;
	
				if (x2 <= x1) {
					//rx, ry表示x,y在投影后对应的点
					int rx = x1, ry = scany;
	
	
					glBegin(GL_POINTS);
					Vector surfacePoint(rx, ry, CurDeep);
					Vector normal(A, B, C);
					calculateColor(surfacePoint,normal,Diffuse,Specular);
	
					glColor3f(Diffuse[0] + Specular[0], Diffuse[1] + Specular[1], Diffuse[2] + Specular[2]);
					if (CurDeep < zb[rx + WIDTH / 2][ry + HEIGHT / 2]) {
						zb[rx + WIDTH / 2][ry + HEIGHT / 2] = CurDeep;
	
						glVertex2i(rx, ry);
					}
					glEnd();
	
				}
				else {
					glBegin(GL_POINTS);
					for (;x1 <= x2;x1++) {
						int rx = x1, ry = scany;
						Vector surfacePoint(rx, ry, CurDeep);
						Vector normal(A, B, C);
//						计算对应的颜色向量 
						calculateColor(surfacePoint,normal,Diffuse,Specular);

						glColor3f(Diffuse[0] + Specular[0], Diffuse[1] + Specular[1], Diffuse[2] + Specular[2]);
						
						if (CurDeep < zb[rx + WIDTH / 2][ry + HEIGHT / 2]) {
	
							zb[rx + WIDTH / 2][ry + HEIGHT / 2] = CurDeep;
							
							//如果是表面模型， 画点
							glVertex2i(rx, ry);
						}
						CurDeep += DeepStep;
					}
	
					glEnd();
				}
				pael = pael->next->next;
			}
			scany++;
			//更新边结点的交点坐标，删除ymax==scany的边//删除AEL链表中指定结点;
			pael = pAEL;
			while (pael && pael->next)
			{
				if (pael->next->ymax == scany)
				{
					EDGE* pt = pael->next;
					pael->next = pael->next->next;
					delete pt;
				}
				else
					pael = pael->next;
			}
			pael = pAEL;
			while (pael)
			{
				pael->xmin = pael->xmin + pael->deltax;
				pael = pael->next;
			}
	
			pael = pAEL->next;
		}
}



void Cube::DDA(int x1,int y1,int x2,int y2,float c1[],float c2[],int scany)
{
//	cout <<"WOWOW"<< endl;
	Vector  tmp1,tmp2;
	Vector position1,position2;
	float Curdeep = -(x1 * A + scany * B + D) / C;
	Vector normal;
	normal.x = A;
	normal.y = B;
	normal.z = C;
	tmp1.setVector(x1,y1,Curdeep);
	tmp2.setVector(x2,y2,Curdeep);
	
//	calculateColor(tmp1,normal,c1);
//	calculateColor(tmp2,normal,c2);

	int dx=x2-x1;
	int dy=y2-y1;
	int dm=abs(x2-x1)>abs(y2-y1)?abs(x2-x1):abs(y2-y1);
	
	float deltax=dx*1.0/dm,deltay=dy*1.0/dm;
	
	float x=x1,y=y1;
	glBegin(GL_POINTS);
	float dr=c2[0]-c1[0];
	float dg=c2[1]-c1[1];
	float db=c2[2]-c1[2];

	for(int d=0;d<dm;d++)
	{
		float r=c1[0]+(x-x1)*1.0/dx*dr;
		float g=c1[1]+(x-x1)*1.0/dx*dg;
		float b=c1[2]+(x-x1)*1.0/dx*db;
		glColor3f(r,g,b);
		glVertex2f(x,y);
		x=x+deltax;
		y=y+deltay;
	}
	glEnd();
}
