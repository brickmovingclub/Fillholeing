
#include "stdafx.h"

#include "CVertexCmp.h"
#include "CVertex.h"

#include "CFacet.h"

#include "CModel.h"
#include "CReader.h"
#include "CPainter.h"
#include "CVector.h"

#include "Common.h""

using namespace ZR;

void test()
{
	CReader reader;	CModel obj;

	//	存储信息
	int pointNum, stage;		//	点的序号，n领域阶
	fs::path fileNameIn = L"D:/stl文件/bunny.stl";
	fs::path fileNameOutPut = L"D:/stl文件/bunnyoutput.stl";
	reader.ReadAscallStl(fileNameIn, obj);
	std::cout << "面片个数：" << obj.GetnumberOfFacets() << std::endl;

	//	寻找孔洞
	std::vector< std::list<std::shared_ptr< CVertex>>> result;
	obj.GetHoles2(result);
	std::shared_ptr< CVertex > ptrA, ptrB, ptrC;

	std::cout << "孔洞的个数：" << result.size() << std::endl;
	for (auto &iter : result)
	{

		std::cout << "组成孔洞的点的个数：" << iter.size() << std::endl;
		for (auto temp : iter)
		{
			std::cout << "孔洞点的坐标：" << temp->GetX() << " " << temp->GetY() << " " << temp->GetZ() << std::endl;
		}
		
		do {
			std::shared_ptr< CFacet> facet = Common::Calangel(iter);
			obj.AddFacet(facet);
		} while (iter.size() > 3);
		std::shared_ptr< CFacet> facet2 = std::make_shared< CFacet>();
		auto iterator = iter.begin();
		facet2->InsertVertex(*iterator++);facet2->InsertVertex(*iterator++);facet2->InsertVertex(*iterator);
		obj.AddFacet(facet2);
			
	}
	std::cout << "修补孔洞后的面片个数：" << obj.GetnumberOfFacets() << std::endl;

	reader.TransformToStl(obj, fileNameOutPut);

}


int main(int argc, char **argv)
{	
	//PlyToStl();
	//OpenGl(argc,argv);
	test();
	system("pause");
	return 0;
}