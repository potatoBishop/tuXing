#include <gl/glut.h>
#include <math.h>
#include"Cube.h"
#include"Matrix.h"
#include <math.h>
#include <stdio.h>
#include<iostream>
#include <gl/glut.h>
const int YDIS=250;//�߱��±�ƫ����
#define LINE 0
#define FILL 1//drawtype=0�߿�,drawtype=1���
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
	static const GLfloat ver[][3] = {// ����λ����������˸����������걣�浽����
		-1.0f, -1.0f, -1.0f,      1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,      1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,      1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,      1.0f,  1.0f,  1.0f};
	static const GLint index[][4] = {// ��ÿ����Ķ�����ű��浽����
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


//���������ÿ����������Ӧ�ñ任����mt
Cube Cube::Mult(Matrix& mt)
{
	Cube cb(*this);
	for(int i=0;i<8;i++)
		cb.Vectors[i]=mt*Vectors[i];
	return cb;

}

//�ñ����޳���ȥ����Ȼ���ɼ����棬�����Ӧ�Ķ���������Ϊ-1
void Cube::CullBack(int type)//type=0����ͶӰ��type=1͸��ͶӰ
{
  //�벹�����
  if(type == 0){
//  	���� 
  	for(int i = 0; i < 6; i ++){

	  	float x1 = Vectors[face[i][1]].x - Vectors[face[i][0]].x;
	  	float y1 = Vectors[face[i][1]].y - Vectors[face[i][0]].y;
	  	float z1 = Vectors[face[i][1]].z - Vectors[face[i][0]].z;
	  	float x2 = Vectors[face[i][2]].x - Vectors[face[i][1]].x;
		float y2 = Vectors[face[i][2]].y - Vectors[face[i][1]].y;
		float z2 = Vectors[face[i][2]].z - Vectors[face[i][1]].z;
		eye = {0,0,-1};
		float z = eye.x*(y1 * z2 - z1 * y2) + eye.y*(z1 * x2 - x1 * z2) + eye.z*(x1 * y2 - y1 * x2);
			//�߿�ͼ����Ҫ���� 
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

// ��������ɫֵ
void calculateColor(Vector& position, Vector& normal,float _Diffuse[3], float _Specular[3])
{
    // ��Դλ�ú���ɫ
    float shininess = 1000.0f;
    
    // ������������ͷ�������
    Vector lightVector;
    lightVector.x = lightPosition.x - position.x;
    lightVector.y = lightPosition.y - position.y;
    lightVector.z = lightPosition.z - position.z;
    lightVector.normalize();
    
    Vector normalVector;
	//���㷨���� 
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

//���������壨���߿�ʽ����䷽ʽ��
//drawtype=LINE�߿�,drawtype=FILL���
void Cube::draw(int drawtype)
{
	lightPosition.setVector(0,0,-5000);
	camera.setCamera(CameraDirection,TargetPosition,CameraUp);
	for(int i =0 ; i < 1005; i ++)
		for(int j = 0; j < 1005; j ++)
			zb[i][j] = 1500;
	//����ÿ���棨����Σ�	
	for(int i=0;i<6;i++)
	{
//		cout << "WO" << endl;
		//���ĳ����ɼ����ͻ��Ƹ����Ӧ�Ķ����,�������������һ�������
		if(face[i][0]<0) 
			continue;
//  		glColor3f(color[i][0],color[i][1],color[i][2]);
			glColor3f(1,0,0);
		if(drawtype==LINE)//˳�����Ӹ�������Ʒ���߿�
		{

			glBegin(GL_LINE_LOOP);//˳�����Ӹ�������Ʒ�ձ߿�
			for(int j=0;j<4;j++){
				float f=Vectors[face[i][j]].w;
				if(fabs(f)<1e-5) continue;//��ֹ����
						
				//����������������ʵ������

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
			//�벹�����
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



//���ݶ�������v[][2]�Ͷ������n�����߱�
void Cube::makeET(int v[][2],int n)


{
	if(n<=0) return ;
	for(int i=0;i<n;i++)
	{             
        int x1=v[i][0],y1=v[i][1];
		int x2=v[(i+1)%n][0],y2=v[(i+1)%n][1];
		if(y1==y2)  continue;
		//����ñߵ�ymin,ymax,xmin, deltax,���ɸñ߽��;
		EDGE *p=new EDGE;
		p->ymax=(y2>y1?y2:y1);
		p->xmin=(y2>y1?x1:x2);
		p->deltax=float(x2-x1)/(y2-y1);
		p->next=NULL;
		int ymin=(y2>y1?y1:y2);

		EDGE *p2=ET[ymin+YDIS];
		//���±߲���ET��Ӧ������֤��������
		if(p2==NULL)   {ET[ymin+YDIS]=p;}//ֱ�Ӳ����ͷ;//����Ϊ����ֱ�Ӳ���
		else
		{
			//�Ƚϵ�ǰ�ߵ�xmin��ET[ymin]�����б߽���xmin�Ĵ�С��ϵ������ǰ�߽����뵽����λ��ep1=��ǰ�߽�㣻ep2=ET[ymin]�е�һ��������
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

//����ET��AEL�Զ���ν���ɨ��ת����vΪ��ά�����������У�nΪ�������,colorΪ�������ɫ
void Cube::drawpolygon(int v[][2],int n,float color[3],float Diffuse[3], float Specular[3])
{
	if (C == 0.0f) return;
		makeET(v, n);
		//Ϊ�˱�֤ET�����±�Ǹ�,����ʱΪy��������ƫ����ydis
	   //�ҵ�ET����С�ǿ�y��;
		int scany = -251;
		for (; scany < 250; scany++) { if (ET[scany + YDIS]) break; }
		while (scany < 250)//scanyΪ��ǰɨ���߶�Ӧyֵ
		{
			//1. ���ET[scany]��ǿ�,�������AEL��
			//2. ��xȡ������ԡ���ɫ
			//3. ����scany,ɾ��ymax==scany�ı�		
			if (ET[scany + YDIS] != NULL)//ET[scany]��ǿ�,�ϲ�������������
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
				break;//����whileѭ������������㷨
	
			EDGE* pael = pAEL->next;//aelheadΪAEL��߱�ͷ���
			while (pael && pael->next)//ȡ������ԡ����
			{
				//����ʵ����Ȼ��������㷨����Ҫ�ڴ˴�������ؼ��㡢�Ƚ�
	
				int x1 = pael->xmin, x2 = pael->next->xmin;
				if (pael->xmin - x1 > 1e-5) x1++;
				if (pael->next->xmin - x2 < 1e-5)x2--;
	
	
				double CurDeep = -(x1 * A + scany * B + D) / C;
				double DeepStep = -A / C;
	
				if (x2 <= x1) {
					//rx, ry��ʾx,y��ͶӰ���Ӧ�ĵ�
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
//						�����Ӧ����ɫ���� 
						calculateColor(surfacePoint,normal,Diffuse,Specular);

						glColor3f(Diffuse[0] + Specular[0], Diffuse[1] + Specular[1], Diffuse[2] + Specular[2]);
						
						if (CurDeep < zb[rx + WIDTH / 2][ry + HEIGHT / 2]) {
	
							zb[rx + WIDTH / 2][ry + HEIGHT / 2] = CurDeep;
							
							//����Ǳ���ģ�ͣ� ����
							glVertex2i(rx, ry);
						}
						CurDeep += DeepStep;
					}
	
					glEnd();
				}
				pael = pael->next->next;
			}
			scany++;
			//���±߽��Ľ������꣬ɾ��ymax==scany�ı�//ɾ��AEL������ָ�����;
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
