
#include "stdafx.h"

#include "Common.h""
#include "CVertexCmp.h"
#include "CModel.h"
#include "CReader.h"
#include "CPainter.h"



using namespace ZR;

void test()
{

	//	�洢��Ϣ
	int pointNum, stage;		//	�����ţ�n�����
	fs::path fileNameIn = L"D:/��Դ/stl�ļ�/bunny.stl";
	fs::path fileNameOut = L"D:/��Դ/����/�������/task2.ply";
	CModel obj;
	int containerType = 3;
	obj.Calculate(fileNameIn, containerType);

	std::cout << "��Ƭ��:" << obj.GetnumberOfFacets() << std::endl;
	std::cout << "�ڵ�����" << obj.GetPoints(containerType) << std::endl;
std:chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	obj.test();

	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	//auto usingTime = endTime - startTime;
	//auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(usingTime);


	//obj.WriteToPlyFile(fileNameOut, containerType);Common
	//std::cout << "ʹ��ʱ��:" << Common::CalculateTimeDiff(startTime, endTime) << "����" << std::endl;




	time_t begin, end;
	while (1)
	{
		std::cout << "�����������point:stage��,example:(100,56)" << std::endl;
		std::cin >> pointNum >> stage;
		begin = clock();
		//	Ѱ���ٽӵ�
		int num = obj.GetTerritory(pointNum, stage);
		end = clock();
		std::cout << "��ڵ����:" << num << std::endl;
		std::cout << "��ʱ��" << (double)(end - begin) / CLOCKS_PER_SEC << std::endl;
	}


}

void PlyToStl()
{
	CModel model;
	CReader reader;
	float area = 0.0f, volum = 0.0f;
	fs::path fileNameIn = L"D:/��Դ/Ply�ļ�/bunny.ply", fileNameOut = L"D:/��Դ/Ply�ļ�/bunny.stl";

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	reader.ReadAscallPly(fileNameIn, model);
	reader.TransformToStl(model, fileNameOut);

	area = model.CalculateSumArea();
	volum = model.CalculateVolume();
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();

	std::cout << "��� =" << area <<"mm2"<<std::endl;
	std::cout << "��� =" << volum << "mm3"<<std::endl;
	std::cout << "��Ƭ��:" << model.GetnumberOfFacets() << std::endl;
	//std::cout << "�ڵ�����" << model.GetPoints(containerType) << std::endl;
	std::cout << "ʹ��ʱ��:" << Common::CalculateTimeDiff(startTime, endTime) << "����" << std::endl;
}


void  OpenGl(int &argc,char **argv)
{
	CReader reader; CModel model;
	unsigned long num = 0;
	unsigned int j = 0;
	fs::path fileName = L"D:/��Դ/����/course7/bunny.ply";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	reader.ReadAscallPly(fileName, model);
	CPainter * painter = CPainter::Instance();
	painter->SetModel(model);
	std::cout << "�����������point:stage��,example:(100,56)" << std::endl;
	std::cin >> num >> j;
	painter->SetPrams(num, j);
	painter->Flush();
	//
	//glutMouseFunc(OnMouseClick);				//	ע������¼�
	glutMainLoop();

}

void TestStlToPly()
{

	//	 vector;
	CReader reader; CModel model;
	fs::path fileName = L"D:/����/course2/bunny.stl";
	fs::path fileNameOut = L"D:/�������/testOut.ply";
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	reader.ReadAscallStl(fileName, model);
	reader.TransformToBinary(model, fileNameOut);
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	std::cout << "vector�洢ʱʹ��ʱ��:" << Common::CalculateTimeDiff(startTime, endTime) << "����" << std::endl;



}
int main(int argc, char **argv)
{	
	//PlyToStl();
	//OpenGl(argc,argv);
	TestStlToPly();
	system("pause");
	return 0;
}