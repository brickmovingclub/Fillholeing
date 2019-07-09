
#include "stdafx.h"
#include "C3Dmodel.h"

#include <chrono>
int main(int argc, char *argv[])
{
	std:chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	ZR::C3Dmodel obj(L"D:/资源/资料/test.stl");
	//obj.Calculate();
	obj.Calculate2();
	std::cout << "面片数:" << obj.GetNumbersOfFacets() << std::endl;
	std::cout << "节点数：" << obj.GetPoints() << std::endl;
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	auto usingTime = endTime - startTime;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(usingTime);



	std::cout << "使用时间:" << ms.count()<<std::endl;
	system("pause");
	return 0;
}