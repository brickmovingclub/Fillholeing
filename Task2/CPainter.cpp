
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
	float viewR = 110.0f, viewH = 0.0f;	//	视点离沿y轴的半径和高度
	float c = 3.1415926535 / 180.0f; //弧度和角度转换参数
	int du = 90; //du是视点绕y轴的角度,opengl里默认y轴
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
		//	清楚缓冲区颜色
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glLoadIdentity();
		gluLookAt(
			viewR * cos(c*du), viewH, viewR * sin(c * du),
			//0.0, 0.0, 30.0,		//相机在世界坐标的位置
			0.0, 0.0, 0.0,				//	相机镜头对准的物体在世界坐标的位置
			0.0, 1.0, 0.0);				//	相机向上的方向在世界坐标中的方向
		//glutWireTeapot(2);
		if(_instance)
			_instance->Draw();
		//glVertexPointer(3, GL_FLOAT, 0, m_ModelData.vecFaceTriangles.data());
		//glColorPointer(3, GL_FLOAT, 0, m_ModelData.vecFaceTriangleColors.data());
		//glDrawArrays(GL_TRIANGLES, 0, m_ModelData.iTotalConnectedTriangles);
		glutSwapBuffers();				//	刷新显示
	}
	static void reshap(int w, int h)
	{
		glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();				//坐标原点在屏幕中心


		//	参数一：视角大小（60度），参数二：实际窗口的纵横比，参数三：近处的切面，参数四：远处的切面
		gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 20, 150);
		glMatrixMode(GL_MODELVIEW);
	}
	static void OnMouseMove(int x, int y)
	{
		//glLoadIdentity();				//坐标原点在屏幕中心
		du += x - _oldX; //鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，这样就左右转了  
		viewH += 0.03f*(y - oldY); //鼠标在窗口y轴方向上的改变加到视点的y坐标上，就上下转了  
		if (viewH > 1.0f) viewH = 1.0f; //视点y坐标作一些限制，不会使视点太奇怪  
		else if (viewH < -1.0f) viewH = -1.0f;
		_oldX = x, oldY = y; //把此时的鼠标坐标作为旧值，为下一次计算增量做准备  
		glutPostRedisplay();
	}
	void CPainter::Draw()
	{
		if (_model == nullptr)
			return;
		 // GL_TRIANGLE_FAN
		glShadeModel(GL_FLAT);
		//	画三角面片
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
		glutDisplayFunc(display);			//	图形显示
		glutReshapeFunc(reshap);			//	窗口实时响应注册函数
		glutMotionFunc(OnMouseMove);			//	响应鼠标拖动
	}

	void CPainter::Colorful()
	{
		
	
		//	给指定点的领域上色
		if (_model == NULL)
			return;
		std::shared_ptr< CVertex> vertex;
		std::set<std::shared_ptr<CVertex>, CVertexCmp>  setback;
		auto pos = std::find_if(_model->_mapModelPoints.begin(), _model->_mapModelPoints.end(), [&](const std::pair<std::shared_ptr< CVertex>, long long> &pairPoint) { return ((pairPoint.second == _num1) ? true : false);   });//ptrVertex->GetNum() == pointNum) ? true : false);
		if (pos == _model->_mapModelPoints.end())
			return ;

		vertex = pos->first;
		_model->GetTerritory(vertex, _num2, setback);
		std::cout << "("<<_num1<<","<<_num2<<")""领域节点数:" << setback.size() << std::endl;
		//vertex->GetNearPoints(setback, setback);
		//std::cout << "领域个数：" << listback.size() << std::endl;

		glPointSize(5.0f);

		//	将输入的点设置为蓝色
		glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(vertex->GetX(), vertex->GetY(), vertex->GetZ());
		glEnd();

		// 将领域的点设置为红色
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


