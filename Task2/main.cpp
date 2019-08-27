
#include "stdafx.h"

#include "CVertexCmp.h"
#include "CModel.h"
#include "CReader.h"
#include "CPainter.h"
#include "Common.h""



using namespace ZR;

void test()
{

	//	存储信息
	int pointNum, stage;		//	点的序号，n领域阶
	fs::path fileNameIn = L"D:/资源/stl文件/bunny.stl";
	fs::path fileNameOut = L"D:/资源/资料/测试输出/task2.ply";
	CModel obj;
	int containerType = 3;
	obj.Calculate(fileNameIn, containerType);

	std::cout << "面片数:" << obj.GetnumberOfFacets() << std::endl;
	std::cout << "节点数：" << obj.GetPoints(containerType) << std::endl;
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