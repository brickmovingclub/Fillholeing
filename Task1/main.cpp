
#include "stdafx.h"
#include "C3Dmodel.h"

#include <chrono>
int main(int argc, char *argv[])
{
	std:chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	ZR::C3Dmodel obj(L"D:/��Դ/����/test.stl");
	//obj.Calculate();
	obj.Calculate2();
	std::cout << "��Ƭ��:" << obj.GetNumbersOfFacets() << std::endl;
	std::cout << "�ڵ�����" << obj.GetPoints() << std::endl;
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	auto usingTime = endTime - startTime;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(usingTime);



	std::cout << "ʹ��ʱ��:" << ms.count()<<std::endl;
	system("pause");
	return 0;
}