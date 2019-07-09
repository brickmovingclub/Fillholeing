
#include "stdafx.h"
#include "CObject.h"


namespace ZR
{
	int _oldX = -1 ,oldY = -1;
	float viewR = 5.0f, viewH = 0.0f;	//	视点离沿y轴的半径和高度
	float c = 3.1415926535 / 180.0f; //弧度和角度转换参数
	int du = 90; //du是视点绕y轴的角度,opengl里默认y轴
	GLfloat angle = 0;

	CObject::CObject()
	{
	}


	CObject::~CObject()
	{
	}
	void CObject::Idle()
	{
		angle += 0.1;
		glutPostRedisplay();
	}

	void CObject::Init(int &argc,char **argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
		glutInitWindowSize(500, 500);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("simple");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		

	}
	void CObject::Show()
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glutDisplayFunc(display);			//	图形显示
		glutReshapeFunc(reshap);			//	窗口实时响应注册函数
		glutMotionFunc(OnMouseMove);			//	响应鼠标拖动
		glutMouseFunc(OnMouseClick);				//	注册鼠标事件

		//glEnable(GL_DEPTH_TEST);       //允许深度检测
		glutMainLoop();
	}
	void CObject::OnMouseClick(int button, int state, int x, int y)
	{
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			_oldX = x;
			oldY = y;
			//glLoadIdentity();
			//glTranslated()
		}			
	}
	void CObject::OnMouseMove( int x, int y)
	{
		//glLoadIdentity();				//坐标原点在屏幕中心
			du += x - _oldX; //鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，这样就左右转了  
			viewH += 0.03f*(y - oldY); //鼠标在窗口y轴方向上的改变加到视点的y坐标上，就上下转了  
			if (viewH > 1.0f) viewH = 1.0f; //视点y坐标作一些限制，不会使视点太奇怪  
			else if (viewH < -1.0f) viewH = -1.0f;
			_oldX = x, oldY = y; //把此时的鼠标坐标作为旧值，为下一次计算增量做准备  
		glutPostRedisplay();
	}

	void CObject::display(void)
	{
		//	清楚缓冲区颜色
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glLoadIdentity();
		std::cout << "x:" << viewR * cos(c*du) << " " << "y:" << viewR * sin(c * du) << std::endl;
		gluLookAt(viewR * cos(c*du), viewH, viewR * sin(c * du),
			//0.0, 0.0, 5.0,		//相机在世界坐标的位置
			0.0, 0.0, 0.0,				//	相机镜头对准的物体在世界坐标的位置
			0.0, 1.0, 0.0);				//	相机向上的方向在世界坐标中的方向
		glutWireTeapot(2);
		glutSwapBuffers();				//	刷新显示
	}

	void CObject::reshap(int w, int h)
	{
		glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();				//坐标原点在屏幕中心


		//	参数一：视角大小（60度），参数二：实际窗口的纵横比，参数三：近处的切面，参数四：远处的切面
		gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
		glMatrixMode(GL_MODELVIEW);
	}
}
