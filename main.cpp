#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>
#include<iostream>
#include"Matrix.h"
#include"Point.h"
#include"Cube.h"
#define LINE 0
#define FILL 1//������Ʒ�ʽ


#define ORTHO		 0//����ͶӰ
#define PERSPECTIVE	 1//͸��ͶӰ
using namespace std;
int ang = 0;
int projectiontype=ORTHO;//��ǰͶӰ����

const double PI=3.14;

float xangle=0;//��Y����ת��
float yangle=0;//��x����ת��
extern Vector CameraPosition;

extern Vector TargetPosition;
Vector CameraRight;
Vector CameraUp;
Vector CameraDirection;
float lastX = 400, lastY = 300;
bool mouseLeftDown;
bool mouseRightDown;
float angle = 90;
void init(void)
{
//	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

Matrix mx;
void display(void)
{
	float tangle = angle / 180*3.14;
	if(ang != 0)
		CameraPosition.setVector(sinf(tangle)*10,10,cosf(tangle)*10);
	CameraDirection = CameraPosition - TargetPosition;
	CameraDirection.normalize();
	cout <<"CameraDirection" << CameraDirection.x <<" " << CameraDirection.y <<" " << CameraDirection.z << endl;
	Vector tmp;
	tmp.setVector(0.0,1.0,0.0);
	CameraRight = tmp.cross(CameraDirection);
	CameraRight.normalize();
	CameraUp = CameraDirection.cross(CameraRight);

	glClear(GL_COLOR_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CameraPosition.x, CameraPosition.y, CameraPosition.z, TargetPosition.x,TargetPosition.y,TargetPosition.z, CameraUp.x,CameraUp.y,CameraUp.z); //�����ӵ�͹۲췽��

	glColor3f(0,0,0);
	
	glBegin(GL_LINES);
		glVertex2f(-500,0);
		glVertex2f( 500,0);
		glVertex2f(0,-500);
		glVertex2f(0, 500);
	glEnd();
	cout <<" �����" << CameraPosition.x<< " "<<CameraPosition.y << " " << CameraPosition.z << endl;

	//����������
	//�����������������Ϊ��0��0��300�����ⳤΪ100
	Cube cube(0,0,300,100);	
	Cube cube2(0, 100, 400, 50);
//	cube.draw(LINE);
//	���岢���ø��������
	Matrix mrotate,mtranslate1,mtranslate2;
	Matrix mprojection;
	Matrix m;
	mtranslate1.setTranslate(0,0,-300);
	mtranslate2.setTranslate(0,0,300);
	if(projectiontype==0)	
		mprojection.setProjection(0);
	else	
		mprojection.setProjection(CameraPosition.z+200);

	//��˳�򽫾��������ˣ��õ����ϱ任����	
	m=mtranslate2*mx*mtranslate1;
	cube=cube.Mult(m);
	cube2=cube2.Mult(m);
	//���޳����Բ��ɼ��ı���
	cube.CullBack(projectiontype);
	cube2.CullBack(projectiontype);
	//�ٽ���ͶӰ�任
	cube=cube.Mult(mprojection);	
	cube2=cube2.Mult(mprojection);	
	cout <<"------------------" << endl;
	//����LINE�߿�ʽ����FILL��ʽ����������
	cube.draw(FILL);
	cube2.draw(FILL);

	glutSwapBuffers();
}

void reshape(int w,int h)
{
	GLfloat aspectRatio; //���崰���߱�
	if(h == 0) h = 1; //����߶Ȳ�����Ϊ0
	aspectRatio=(GLfloat)w/(GLfloat)h;

	glViewport(0,0,700,700);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2,w-1-w/2,-h/2,h-1-h/2,-700,700);
}

//���̻ص������У�������йػ��ƵĴ���
//���̻ص�����--������key������ʱ��ִ�иú�������//�����������������������µļ������¸ü�ʱ����λ�ã�x,y��
void myKeyFunc(unsigned char key,int x,int y)
{
	
	Vector cameraFront;
	Vector cameraUp;
	Vector cameraDown;
	cameraFront.setVector(0,0,10);
	cameraUp.setVector(0,10,0);
	cameraDown.setVector(0,-10,0);

	//������¼���q/Q�����˳�����
	if(key=='q'||key=='Q')	exit(0);
	if(key==' ')
	{
		projectiontype=PERSPECTIVE+ORTHO-projectiontype;//�л�ͶӰ��ʽ
			glutPostRedisplay();
	}
	if(key == 'w' || key == 'W'){
		CameraPosition = CameraPosition + cameraFront;
			glutPostRedisplay();
	}	
	if(key == 's' || key =='S'){
		CameraPosition = CameraPosition - cameraFront;
			glutPostRedisplay();
	}
	if (key =='a' || key == 'A'){
		Vector tmp = cameraFront.cross(cameraUp);
		tmp.normalize();
		CameraPosition = CameraPosition - tmp;
		glutPostRedisplay();		
	}
	if (key =='d' || key == 'D'){
		Vector tmp = cameraFront.cross(cameraDown);
		tmp.normalize();
		CameraPosition = CameraPosition + tmp;
		glutPostRedisplay();
	}		
	if(key == 'x' || key == 'X'){
		if(ang)
			ang = 0;
		else
			ang = 2;

	}

}

//���̻ص������У�������йػ��ƵĴ���
//������ص�����--���������F1��F2��������Ҽ����ϼ����¼����ȱ�����ʱ��ִ�иú�������
void mySpecialFunc(int key,int x,int y)
{
	int preangle=yangle;

	//���ҷ������������������Y����ת
	if(key==GLUT_KEY_RIGHT)
	{
		yangle-=5;
		if(yangle>=360) yangle=0;
		//�����û������ı�ȫ�ֱ任����mx
		Matrix m1;
		m1.setYRotate(yangle-preangle);//����ʵ����ת�Ƕȼ�����ת�任����
		mx=m1*mx;
	}
	else if(key==GLUT_KEY_LEFT)
	{
		yangle+=5;
		if(xangle>=360) yangle=0;
		//�����û������ı�ȫ�ֱ任����mx
		Matrix m1;
		m1.setYRotate(yangle-preangle);//����ʵ����ת�Ƕȼ�����ת�任����
		mx=m1*mx;
	}

	preangle=xangle;
	//���·������������������X����ת	
	if(key==GLUT_KEY_UP)
	{
		xangle-=5;
		if(xangle>=360) xangle=0;
		Matrix m1;
		m1.setXRotate(xangle-preangle);//����ʵ����ת�Ƕȼ�����ת�任����
		mx=m1*mx;
	}
	else if(key==GLUT_KEY_DOWN)
	{
		xangle+=5;
		if(xangle>=360) xangle=0;
		Matrix m1;
		m1.setXRotate(xangle-preangle);//����ʵ����ת�Ƕȼ�����ת�任����
		mx=m1*mx;
	}

	glutPostRedisplay();
}
void mouseCB(int button, int state, int x, int y)
{
    lastX = x;
   	lastY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
//        cameraAngleY += (x - mouseX);
//        cameraAngleX += (y - mouseY);

//        mouseX = x;
//        mouseY = y;
    }

    glutPostRedisplay();
}
void timerFunc(int nTimerID)
{
    switch(nTimerID)
    {
        case 1:
            angle= angle+ang; 
            glutPostRedisplay();
            glutTimerFunc(40,timerFunc,1);
        break;
    }
}


int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);//GLUT_DOUBLE��˫����
	glutInitWindowSize(700,700);

	CameraPosition.setVector(0,0,1);
	TargetPosition.setVector(0,0,0);
	glutCreateWindow("ͶӰ�任");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeyFunc);//ע����̻ص����������̻ص�����Ϊ�û��Զ��庯��mykey()
	glutSpecialFunc(mySpecialFunc);//ע��������ص�������������ص�����Ϊ�û��Զ��庯��myspecial()
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutTimerFunc(40,timerFunc,1);
	glutMainLoop();
}
