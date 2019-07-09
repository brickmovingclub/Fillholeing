

#include "stdafx.h"
#include "CObject.h"


using namespace ZR;
/*
需求：绘制一个简单的地月太阳的旋转模型：地球绕太阳（绕y轴）旋转，月球绕地球（绕x轴）旋转。

实现的步骤：
	//openGL 坐标系：z朝向屏幕外， y朝上，x朝右
push
	向里移动300

	画了一颗太阳

	绕y轴旋转坐标系，旋转角度为fEarthRot //（此时太阳的坐标系已经在绕太阳中心点（绕x轴）旋转呢，不管后面有没有下一步。理解这点很重要。）

	向上移动105

	画一个颗地球 //（这时地球自然而然在绕y轴旋转，半径为上面移动的105。）

	绕x轴旋转坐标系，旋转角度为fMoonRot //（此时地球的坐标系在绕地球中心点（绕x轴）旋转。）

	向上移动30

	画一颗月亮 //（这时地球自然而然在绕x轴旋转，半径为上面移动的30。）
pop //（物体坐标系又回到世界坐标系原点）

	//注意它两是静态变量
	fEarthRot++
	fMoonRot++

	循环上面这动作，实现了动画。
//#include<gl/glut.h>
#include <math.h>

// Lighting values
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat     lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

void createCoordinate();

//执行顺序： main --> SetupRC --> TimerFunc --> ChangeSize -->RenderScene
// Called to draw scene
void RenderScene(void)
{
	//openGL 坐标系：z朝向屏幕外， y朝上，x朝右
	// Earth and Moon angle of revolution
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Translate the whole scene out and into view    
	//向里移动300
	glTranslatef(0.0f, 0.0f, -300.0f);
	createCoordinate();

	// Set material color, Red
	// Sun 画了一颗太阳
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 0);
	glutSolidSphere(15.0f, 30, 17);
	glEnable(GL_LIGHTING);

	// Move the light after we draw the sun!
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// 绕y轴旋转坐标系
	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);

	//向上移动105 
	glTranslatef(105.0f, 0.0f, 0.0f);

	//画一个颗地球
	glColor3ub(0, 0, 255);
	glutSolidSphere(15.0f, 30, 17);

	//绘制一个坐标系，
	createCoordinate();

	// Rotate from Earth based coordinates and draw Moon
	glColor3ub(200, 200, 200);
	//绕x轴旋转
	glRotatef(fMoonRot, 1.0f, 0.0f, 0.0f);
	//向上移动30
	glTranslatef(0.0f, 30.0f, 0.0f);

	//画一颗月亮
	glutSolidSphere(6.0f, 30, 17);

	createCoordinate();

	// Restore the matrix state
	glPopMatrix();    // Modelview matrix

	//月亮在转
	fMoonRot += 5.0f;
	if (fMoonRot > 360.0f)
		fMoonRot = 0.0f;
	// Step earth orbit 5 degrees

	//地球在转
	fEarthRot += 5.0f;
	if (fEarthRot > 360.0f)
		fEarthRot = 0.0f;

	// Show the image
	glutSwapBuffers();
}

void createCoordinate() {
	//绘制一个坐标系，

	GLint factor = 2;
	GLushort pattern = 0x5555;
	//启用点画    
	glEnable(GL_LINE_STIPPLE);
	//点画模式
	glLineStipple(factor, pattern);

	glLineWidth(1.0f);

	glBegin(GL_LINES);
	//x轴
	glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);

	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);

	//y轴
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	//z轴
	glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)255);

	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	//关闭点画    
	glDisable(GL_LINE_STIPPLE);
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Light values and coordinates
	glEnable(GL_DEPTH_TEST);    // Hidden surface removal
	glFrontFace(GL_CCW);        // Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);        // Do not calculate inside of jet

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc, 1);
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	if (h == 0)h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	//使用并初始化投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, fAspect, 1.0, 425.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Earth/Moon/Sun System");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}

*/

int main(int argc, char **argv)
{
	

	CObject model;
	model.Init(argc, argv);
	model.Show();
	return 0;
}