#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>
#include<iostream>
#include"Matrix.h"
#include"Point.h"
#include"Cube.h"
#define LINE 0
#define FILL 1//物体绘制方式


#define ORTHO		 0//正交投影
#define PERSPECTIVE	 1//透视投影
using namespace std;
int ang = 0;
int projectiontype=ORTHO;//当前投影类型

const double PI=3.14;

float xangle=0;//绕Y轴旋转角
float yangle=0;//绕x轴旋转角
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
	gluLookAt(CameraPosition.x, CameraPosition.y, CameraPosition.z, TargetPosition.x,TargetPosition.y,TargetPosition.z, CameraUp.x,CameraUp.y,CameraUp.z); //设置视点和观察方向

	glColor3f(0,0,0);
	
	glBegin(GL_LINES);
		glVertex2f(-500,0);
		glVertex2f( 500,0);
		glVertex2f(0,-500);
		glVertex2f(0, 500);
	glEnd();
	cout <<" 摄像机" << CameraPosition.x<< " "<<CameraPosition.y << " " << CameraPosition.z << endl;

	//绘制立方体
	//定义立方体对象，中心为（0，0，300），棱长为100
	Cube cube(0,0,300,100);	
	Cube cube2(0, 100, 400, 50);
//	cube.draw(LINE);
//	定义并设置各矩阵对象
	Matrix mrotate,mtranslate1,mtranslate2;
	Matrix mprojection;
	Matrix m;
	mtranslate1.setTranslate(0,0,-300);
	mtranslate2.setTranslate(0,0,300);
	if(projectiontype==0)	
		mprojection.setProjection(0);
	else	
		mprojection.setProjection(CameraPosition.z+200);

	//按顺序将矩阵对象相乘，得到复合变换矩阵	
	m=mtranslate2*mx*mtranslate1;
	cube=cube.Mult(m);
	cube2=cube2.Mult(m);
	//先剔除明显不可见的背面
	cube.CullBack(projectiontype);
	cube2.CullBack(projectiontype);
	//再进行投影变换
	cube=cube.Mult(mprojection);	
	cube2=cube2.Mult(mprojection);	
	cout <<"------------------" << endl;
	//按照LINE线框方式或者FILL方式绘制立方体
	cube.draw(FILL);
	cube2.draw(FILL);

	glutSwapBuffers();
}

void reshape(int w,int h)
{
	GLfloat aspectRatio; //定义窗体宽高比
	if(h == 0) h = 1; //窗体高度不允许为0
	aspectRatio=(GLfloat)w/(GLfloat)h;

	glViewport(0,0,700,700);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2,w-1-w/2,-h/2,h-1-h/2,-700,700);
}

//键盘回调函数中，不添加有关绘制的代码
//键盘回调函数--当按键key被按下时，执行该函数代码//函数必须有三个参数：按下的键，按下该键时光标的位置（x,y）
void myKeyFunc(unsigned char key,int x,int y)
{
	
	Vector cameraFront;
	Vector cameraUp;
	Vector cameraDown;
	cameraFront.setVector(0,0,10);
	cameraUp.setVector(0,10,0);
	cameraDown.setVector(0,-10,0);

	//如果按下键盘q/Q，则退出程序
	if(key=='q'||key=='Q')	exit(0);
	if(key==' ')
	{
		projectiontype=PERSPECTIVE+ORTHO-projectiontype;//切换投影方式
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

//键盘回调函数中，不添加有关绘制的代码
//特殊键回调函数--当特殊键如F1、F2、左键（右键、上键、下键）等被按下时，执行该函数代码
void mySpecialFunc(int key,int x,int y)
{
	int preangle=yangle;

	//左右方向键，控制物体绕着Y轴旋转
	if(key==GLUT_KEY_RIGHT)
	{
		yangle-=5;
		if(yangle>=360) yangle=0;
		//根据用户操作改变全局变换矩阵mx
		Matrix m1;
		m1.setYRotate(yangle-preangle);//根据实际旋转角度计算旋转变换矩阵
		mx=m1*mx;
	}
	else if(key==GLUT_KEY_LEFT)
	{
		yangle+=5;
		if(xangle>=360) yangle=0;
		//根据用户操作改变全局变换矩阵mx
		Matrix m1;
		m1.setYRotate(yangle-preangle);//根据实际旋转角度计算旋转变换矩阵
		mx=m1*mx;
	}

	preangle=xangle;
	//上下方向键，控制物体绕着X轴旋转	
	if(key==GLUT_KEY_UP)
	{
		xangle-=5;
		if(xangle>=360) xangle=0;
		Matrix m1;
		m1.setXRotate(xangle-preangle);//根据实际旋转角度计算旋转变换矩阵
		mx=m1*mx;
	}
	else if(key==GLUT_KEY_DOWN)
	{
		xangle+=5;
		if(xangle>=360) xangle=0;
		Matrix m1;
		m1.setXRotate(xangle-preangle);//根据实际旋转角度计算旋转变换矩阵
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
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);//GLUT_DOUBLE，双缓存
	glutInitWindowSize(700,700);

	CameraPosition.setVector(0,0,1);
	TargetPosition.setVector(0,0,0);
	glutCreateWindow("投影变换");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeyFunc);//注册键盘回调函数，键盘回调函数为用户自定义函数mykey()
	glutSpecialFunc(mySpecialFunc);//注册特殊键回调函数，特殊键回调函数为用户自定义函数myspecial()
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutTimerFunc(40,timerFunc,1);
	glutMainLoop();
}
