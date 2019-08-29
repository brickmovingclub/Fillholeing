
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

	//	�洢��Ϣ
	int pointNum, stage;		//	�����ţ�n�����
	fs::path fileNameIn = L"D:/stl�ļ�/bunny.stl";
	fs::path fileNameOutPut = L"D:/stl�ļ�/bunnyoutput.stl";
	reader.ReadAscallStl(fileNameIn, obj);
	std::cout << "��Ƭ������" << obj.GetnumberOfFacets() << std::endl;

	//	Ѱ�ҿ׶�
	std::vector< std::list<std::shared_ptr< CVertex>>> result;
	obj.GetHoles2(result);
	std::shared_ptr< CVertex > ptrA, ptrB, ptrC;

	std::cout << "�׶��ĸ�����" << result.size() << std::endl;
	for (auto &iter : result)
	{

		std::cout << "��ɿ׶��ĵ�ĸ�����" << iter.size() << std::endl;
		for (auto temp : iter)
		{
			std::cout << "�׶�������꣺" << temp->GetX() << " " << temp->GetY() << " " << temp->GetZ() << std::endl;
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
	std::cout << "�޲��׶������Ƭ������" << obj.GetnumberOfFacets() << std::endl;

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