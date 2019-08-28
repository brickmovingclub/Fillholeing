
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
	
	for (auto iter : result)
	{

		std::cout << "组成孔洞的点的个数：" << iter.size() << std::endl;
		for (auto temp : iter)
		{
			std::cout << "孔洞点的坐标：" << temp->GetX() << " " << temp->GetY() << " " << temp->GetZ() << std::endl;
		}
		std::list<std::shared_ptr< CVertex>>::iterator it = iter.begin();
		std::list<std::shared_ptr< CVertex>>::iterator move;
		do {
			move = it;
			if (move != iter.end())
				ptrA = *move++;
			//else
			//	break;
			if (move != iter.end())
				ptrB = *move++;
			//else
			//	break;
			if (move != iter.end())
				ptrC = *move++;
			//else
			//	break;			

			int  type =  Common::Calangel(ptrA, ptrB, ptrC);
			switch (type)
			{
			case LESS90:
			{
				std::shared_ptr< CFacet> facet = std::make_shared<CFacet>();
				facet->InsertVertex(ptrA); facet->InsertVertex(ptrC); facet->InsertVertex(ptrB);
				obj.AddFacet(facet);
				//	更新孔洞边
				auto iterator = iter.erase(std::find(iter.begin(), iter.end(), ptrB));
				ptrB = *iterator;
				ptrC = *(iterator++);
				break;
			}
			case LESS180ANDBIGGER90:
			{
				std::shared_ptr< CVertex> addvertex = std::make_shared< CVertex>();
				Common::CalTriangleCricumcenter(ptrA, ptrB, ptrC, addvertex);
				std::cout << "外切圆的坐标：" << addvertex->GetX() << " " << addvertex->GetY() << " " << addvertex->GetZ() << std::endl;

				if (Common::VertexIsValid(ptrA, ptrB, addvertex, iter))
				//if(true)
				{
				
					std::shared_ptr< CFacet> facet1 = std::make_shared<CFacet>();
					std::shared_ptr< CFacet> facet2 = std::make_shared<CFacet>();

					facet1->InsertVertex(ptrA); facet1->InsertVertex(ptrB); facet1->InsertVertex(addvertex);
					obj.AddFacet(facet1);
					facet2->InsertVertex(ptrB); facet2->InsertVertex(ptrC); facet2->InsertVertex(addvertex);
					obj.AddFacet(facet2);
					//	更新孔洞边
					ptrB->SetVertex(addvertex->GetX(), addvertex->GetY(), addvertex->GetZ(), 0);
				}
				break;
			}
			case BIGGER180:
				break;
			default:
				break;
			}

			it++;
			
		} while (move != iter.end());
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