
#include "stdafx.h"

#include "CVertexCmp.h"
#include "CModel.h"
#include "CReader.h"
#include "CPainter.h"
#include "Common.h""



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
}


int main(int argc, char **argv)
{	
	//PlyToStl();
	//OpenGl(argc,argv);
	test();
	system("pause");
	return 0;
}