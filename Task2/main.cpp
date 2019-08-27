
#include "stdafx.h"

#include "Common.h""
#include "CVertexCmp.h"
#include "CModel.h"
#include "CReader.h"
#include "CPainter.h"



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

	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	//auto usingTime = endTime - startTime;
	//auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(usingTime);


	//obj.WriteToPlyFile(fileNameOut, containerType);Common
	//std::cout << "使用时间:" << Common::CalculateTimeDiff(startTime, endTime) << "毫秒" << std::endl;




	time_t begin, end;
	while (1)
	{
		std::cout << "请输入参数（point:stage）,example:(100,56)" << std::endl;
		std::cin >> pointNum >> stage;
		begin = clock();
		//	寻找临接点
		int num = obj.GetTerritory(pointNum, stage);
		end = clock();
		std::cout << "领节点个数:" << num << std::endl;
		std::cout << "耗时：" << (double)(end - begin) / CLOCKS_PER_SEC << std::endl;
	}


}

void PlyToStl()
{
	CModel model;
	CReader reader;
	float area = 0.0f, volum = 0.0f;
	fs::path fileNameIn = L"D:/资源/Ply文件/bunny.ply", fileNameOut = L"D:/资源/Ply文件/bunny.stl";

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	reader.ReadAscallPly(fileNameIn, model);
	reader.TransformToStl(model, fileNameOut);

	area = model.CalculateSumArea();
	volum = model.CalculateVolume();
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();

	std::cout << "面积 =" << area <<"mm2"<<std::endl;
	std::cout << "体积 =" << volum << "mm3"<<std::endl;
	std::cout << "面片数:" << model.GetnumberOfFacets() << std::endl;
	//std::cout << "节点数：" << model.GetPoints(containerType) << std::endl;
	std::cout << "使用时间:" << Common::CalculateTimeDiff(startTime, endTime) << "毫秒" << std::endl;
}


void  OpenGl(int &argc,char **argv)
{
	CReader reader; CModel model;
	unsigned long num = 0;
	unsigned int j = 0;
	fs::path fileName = L"D:/资源/资料/course7/bunny.ply";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	reader.ReadAscallPly(fileName, model);
	CPainter * painter = CPainter::Instance();
	painter->SetModel(model);
	std::cout << "请输入参数（point:stage）,example:(100,56)" << std::endl;
	std::cin >> num >> j;
	painter->SetPrams(num, j);
	painter->Flush();
	//
	//glutMouseFunc(OnMouseClick);				//	注册鼠标事件
	glutMainLoop();

}

void TestStlToPly()
{

	//	 vector;
	CReader reader; CModel model;
	fs::path fileName = L"D:/资料/course2/bunny.stl";
	fs::path fileNameOut = L"D:/测试输出/testOut.ply";
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	reader.ReadAscallStl(fileName, model);
	reader.TransformToBinary(model, fileNameOut);
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	std::cout << "vector存储时使用时间:" << Common::CalculateTimeDiff(startTime, endTime) << "毫秒" << std::endl;



}
int main(int argc, char **argv)
{	
	//PlyToStl();
	//OpenGl(argc,argv);
	TestStlToPly();
	system("pause");
	return 0;
}