
#include "stdafx.h"
#include "CObject.h"


namespace ZR
{
	int _oldX = -1 ,oldY = -1;
	float viewR = 5.0f, viewH = 0.0f;	//	�ӵ�����y��İ뾶�͸߶�
	float c = 3.1415926535 / 180.0f; //���ȺͽǶ�ת������
	int du = 90; //du���ӵ���y��ĽǶ�,opengl��Ĭ��y��
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
		glutDisplayFunc(display);			//	ͼ����ʾ
		glutReshapeFunc(reshap);			//	����ʵʱ��Ӧע�ắ��
		glutMotionFunc(OnMouseMove);			//	��Ӧ����϶�
		glutMouseFunc(OnMouseClick);				//	ע������¼�

		//glEnable(GL_DEPTH_TEST);       //������ȼ��
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
		//glLoadIdentity();				//����ԭ������Ļ����
			du += x - _oldX; //����ڴ���x�᷽���ϵ������ӵ��ӵ���y��ĽǶ��ϣ�����������ת��  
			viewH += 0.03f*(y - oldY); //����ڴ���y�᷽���ϵĸı�ӵ��ӵ��y�����ϣ�������ת��  
			if (viewH > 1.0f) viewH = 1.0f; //�ӵ�y������һЩ���ƣ�����ʹ�ӵ�̫���  
			else if (viewH < -1.0f) viewH = -1.0f;
			_oldX = x, oldY = y; //�Ѵ�ʱ�����������Ϊ��ֵ��Ϊ��һ�μ���������׼��  
		glutPostRedisplay();
	}

	void CObject::display(void)
	{
		//	�����������ɫ
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glLoadIdentity();
		std::cout << "x:" << viewR * cos(c*du) << " " << "y:" << viewR * sin(c * du) << std::endl;
		gluLookAt(viewR * cos(c*du), viewH, viewR * sin(c * du),
			//0.0, 0.0, 5.0,		//��������������λ��
			0.0, 0.0, 0.0,				//	�����ͷ��׼�����������������λ��
			0.0, 1.0, 0.0);				//	������ϵķ��������������еķ���
		glutWireTeapot(2);
		glutSwapBuffers();				//	ˢ����ʾ
	}

	void CObject::reshap(int w, int h)
	{
		glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();				//����ԭ������Ļ����


		//	����һ���ӽǴ�С��60�ȣ�����������ʵ�ʴ��ڵ��ݺ�ȣ������������������棬�����ģ�Զ��������
		gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
		glMatrixMode(GL_MODELVIEW);
	}
}
