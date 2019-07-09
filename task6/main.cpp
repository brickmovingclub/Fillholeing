

#include "stdafx.h"
#include "CObject.h"


using namespace ZR;
/*
���󣺻���һ���򵥵ĵ���̫������תģ�ͣ�������̫������y�ᣩ��ת�������Ƶ�����x�ᣩ��ת��

ʵ�ֵĲ��裺
	//openGL ����ϵ��z������Ļ�⣬ y���ϣ�x����
push
	�����ƶ�300

	����һ��̫��

	��y����ת����ϵ����ת�Ƕ�ΪfEarthRot //����ʱ̫��������ϵ�Ѿ�����̫�����ĵ㣨��x�ᣩ��ת�أ����ܺ�����û����һ�������������Ҫ����

	�����ƶ�105

	��һ���ŵ��� //����ʱ������Ȼ��Ȼ����y����ת���뾶Ϊ�����ƶ���105����

	��x����ת����ϵ����ת�Ƕ�ΪfMoonRot //����ʱ���������ϵ���Ƶ������ĵ㣨��x�ᣩ��ת����

	�����ƶ�30

	��һ������ //����ʱ������Ȼ��Ȼ����x����ת���뾶Ϊ�����ƶ���30����
pop //����������ϵ�ֻص���������ϵԭ�㣩

	//ע�������Ǿ�̬����
	fEarthRot++
	fMoonRot++

	ѭ�������⶯����ʵ���˶�����
//#include<gl/glut.h>
#include <math.h>

// Lighting values
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat     lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

void createCoordinate();

//ִ��˳�� main --> SetupRC --> TimerFunc --> ChangeSize -->RenderScene
// Called to draw scene
void RenderScene(void)
{
	//openGL ����ϵ��z������Ļ�⣬ y���ϣ�x����
	// Earth and Moon angle of revolution
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Translate the whole scene out and into view    
	//�����ƶ�300
	glTranslatef(0.0f, 0.0f, -300.0f);
	createCoordinate();

	// Set material color, Red
	// Sun ����һ��̫��
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 0);
	glutSolidSphere(15.0f, 30, 17);
	glEnable(GL_LIGHTING);

	// Move the light after we draw the sun!
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// ��y����ת����ϵ
	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);

	//�����ƶ�105 
	glTranslatef(105.0f, 0.0f, 0.0f);

	//��һ���ŵ���
	glColor3ub(0, 0, 255);
	glutSolidSphere(15.0f, 30, 17);

	//����һ������ϵ��
	createCoordinate();

	// Rotate from Earth based coordinates and draw Moon
	glColor3ub(200, 200, 200);
	//��x����ת
	glRotatef(fMoonRot, 1.0f, 0.0f, 0.0f);
	//�����ƶ�30
	glTranslatef(0.0f, 30.0f, 0.0f);

	//��һ������
	glutSolidSphere(6.0f, 30, 17);

	createCoordinate();

	// Restore the matrix state
	glPopMatrix();    // Modelview matrix

	//������ת
	fMoonRot += 5.0f;
	if (fMoonRot > 360.0f)
		fMoonRot = 0.0f;
	// Step earth orbit 5 degrees

	//������ת
	fEarthRot += 5.0f;
	if (fEarthRot > 360.0f)
		fEarthRot = 0.0f;

	// Show the image
	glutSwapBuffers();
}

void createCoordinate() {
	//����һ������ϵ��

	GLint factor = 2;
	GLushort pattern = 0x5555;
	//���õ㻭    
	glEnable(GL_LINE_STIPPLE);
	//�㻭ģʽ
	glLineStipple(factor, pattern);

	glLineWidth(1.0f);

	glBegin(GL_LINES);
	//x��
	glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);

	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);

	//y��
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	//z��
	glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)255);

	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	//�رյ㻭    
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

	//ʹ�ò���ʼ��ͶӰ����
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