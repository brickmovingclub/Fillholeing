
#include "stdafx.h"
#include "Common.h""
#include "CVertexCmp.h"

#include "CVertex.h"
#include "CFacet.h"
#include "CModel.h"

#include "CPainter.h"

namespace ZR
{
	static CPainter * _instance = NULL;
	static mutex _mutexInstance;

	int _oldX = -1, oldY = -1;
	float viewR = 110.0f, viewH = 0.0f;	//	�ӵ�����y��İ뾶�͸߶�
	float c = 3.1415926535 / 180.0f; //���ȺͽǶ�ת������
	int du = 90; //du���ӵ���y��ĽǶ�,opengl��Ĭ��y��
	CPainter::CPainter():_model(nullptr)
	{
	}


	CPainter::~CPainter()
	{
	}

	CPainter *CPainter::Instance()
	{
		if (_instance == NULL)
		{
			lock_guard<mutex> lockLoacl(_mutexInstance);
			if (_instance == NULL)
			{
				auto p = new CPainter;
				//BaseScript::LoadAllInlineScripts(*p);
				_instance = p;
			}
		}
		return _instance;
	}

	void CPainter::SetModel(CModel &model)
	{
		_model = &model;
	}

	static void display(void)
	{
		//	�����������ɫ
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glLoadIdentity();
		gluLookAt(
			viewR * cos(c*du), viewH, viewR * sin(c * du),
			//0.0, 0.0, 30.0,		//��������������λ��
			0.0, 0.0, 0.0,				//	�����ͷ��׼�����������������λ��
			0.0, 1.0, 0.0);				//	������ϵķ��������������еķ���
		//glutWireTeapot(2);
		if(_instance)
			_instance->Draw();
		//glVertexPointer(3, GL_FLOAT, 0, m_ModelData.vecFaceTriangles.data());
		//glColorPointer(3, GL_FLOAT, 0, m_ModelData.vecFaceTriangleColors.data());
		//glDrawArrays(GL_TRIANGLES, 0, m_ModelData.iTotalConnectedTriangles);
		glutSwapBuffers();				//	ˢ����ʾ
	}
	static void reshap(int w, int h)
	{
		glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();				//����ԭ������Ļ����


		//	����һ���ӽǴ�С��60�ȣ�����������ʵ�ʴ��ڵ��ݺ�ȣ������������������棬�����ģ�Զ��������
		gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 20, 150);
		glMatrixMode(GL_MODELVIEW);
	}
	static void OnMouseMove(int x, int y)
	{
		//glLoadIdentity();				//����ԭ������Ļ����
		du += x - _oldX; //����ڴ���x�᷽���ϵ������ӵ��ӵ���y��ĽǶ��ϣ�����������ת��  
		viewH += 0.03f*(y - oldY); //����ڴ���y�᷽���ϵĸı�ӵ��ӵ��y�����ϣ�������ת��  
		if (viewH > 1.0f) viewH = 1.0f; //�ӵ�y������һЩ���ƣ�����ʹ�ӵ�̫���  
		else if (viewH < -1.0f) viewH = -1.0f;
		_oldX = x, oldY = y; //�Ѵ�ʱ�����������Ϊ��ֵ��Ϊ��һ�μ���������׼��  
		glutPostRedisplay();
	}
	void CPainter::Draw()
	{
		if (_model == nullptr)
			return;
		 // GL_TRIANGLE_FAN
		glShadeModel(GL_FLAT);
		//	��������Ƭ
		for (auto iter : _model->_listFacets)
		{
			glBegin(GL_POLYGON);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(iter->GetVertex(0)->GetX(), iter->GetVertex(0)->GetY(), iter->GetVertex(0)->GetZ());
			glVertex3f(iter->GetVertex(1)->GetX(), iter->GetVertex(1)->GetY(), iter->GetVertex(1)->GetZ());
			glVertex3f(iter->GetVertex(2)->GetX(), iter->GetVertex(2)->GetY(), iter->GetVertex(2)->GetZ());
			glEnd();

			
			glBegin(GL_LINES);
			glColor3f(0.8, 0.8f, 0.8f);
			glVertex3f(iter->GetVertex(0)->GetX(), iter->GetVertex(0)->GetY(), iter->GetVertex(0)->GetZ());
			glVertex3f(iter->GetVertex(1)->GetX(), iter->GetVertex(1)->GetY(), iter->GetVertex(1)->GetZ());
			glVertex3f(iter->GetVertex(1)->GetX(), iter->GetVertex(1)->GetY(), iter->GetVertex(1)->GetZ());
			glVertex3f(iter->GetVertex(2)->GetX(), iter->GetVertex(2)->GetY(), iter->GetVertex(2)->GetZ());
			glVertex3f(iter->GetVertex(2)->GetX(), iter->GetVertex(2)->GetY(), iter->GetVertex(2)->GetZ());
			glVertex3f(iter->GetVertex(0)->GetX(), iter->GetVertex(0)->GetY(), iter->GetVertex(0)->GetZ());

			glEnd();
			
		}
	
		Colorful();

	}
	void CPainter::Flush()
	{
		
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glutDisplayFunc(display);			//	ͼ����ʾ
		glutReshapeFunc(reshap);			//	����ʵʱ��Ӧע�ắ��
		glutMotionFunc(OnMouseMove);			//	��Ӧ����϶�
	}

	void CPainter::Colorful()
	{
		
	
		//	��ָ�����������ɫ
		if (_model == NULL)
			return;
		std::shared_ptr< CVertex> vertex;
		std::set<std::shared_ptr<CVertex>, CVertexCmp>  setback;
		auto pos = std::find_if(_model->_mapModelPoints.begin(), _model->_mapModelPoints.end(), [&](const std::pair<std::shared_ptr< CVertex>, long long> &pairPoint) { return ((pairPoint.second == _num1) ? true : false);   });//ptrVertex->GetNum() == pointNum) ? true : false);
		if (pos == _model->_mapModelPoints.end())
			return ;

		vertex = pos->first;
		_model->GetTerritory(vertex, _num2, setback);
		std::cout << "("<<_num1<<","<<_num2<<")""����ڵ���:" << setback.size() << std::endl;
		//vertex->GetNearPoints(setback, setback);
		//std::cout << "���������" << listback.size() << std::endl;

		glPointSize(5.0f);

		//	������ĵ�����Ϊ��ɫ
		glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(vertex->GetX(), vertex->GetY(), vertex->GetZ());
		glEnd();

		// ������ĵ�����Ϊ��ɫ
		for (auto iter : setback)
		{
			//std::cout << iter->GetNum() << std::endl;
			glBegin(GL_POINTS);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(iter->GetX(), iter->GetY(), iter->GetZ());
			glEnd();

		}

		setback.clear();
	}
	void CPainter::SetPrams(const unsigned long &num, const unsigned int num2)
	{
		_num1 = num;
		_num2 = num2;
	}
}


