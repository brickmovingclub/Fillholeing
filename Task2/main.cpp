
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
	
	for (auto iter : result)
	{

		std::cout << "��ɿ׶��ĵ�ĸ�����" << iter.size() << std::endl;
		for (auto temp : iter)
		{
			std::cout << "�׶�������꣺" << temp->GetX() << " " << temp->GetY() << " " << temp->GetZ() << std::endl;
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
				//	���¿׶���
				auto iterator = iter.erase(std::find(iter.begin(), iter.end(), ptrB));
				ptrB = *iterator;
				ptrC = *(iterator++);
				break;
			}
			case LESS180ANDBIGGER90:
			{
				std::shared_ptr< CVertex> addvertex = std::make_shared< CVertex>();
				Common::CalTriangleCricumcenter(ptrA, ptrB, ptrC, addvertex);
				std::cout << "����Բ�����꣺" << addvertex->GetX() << " " << addvertex->GetY() << " " << addvertex->GetZ() << std::endl;

				if (Common::VertexIsValid(ptrA, ptrB, addvertex, iter))
				//if(true)
				{
				
					std::shared_ptr< CFacet> facet1 = std::make_shared<CFacet>();
					std::shared_ptr< CFacet> facet2 = std::make_shared<CFacet>();

					facet1->InsertVertex(ptrA); facet1->InsertVertex(ptrB); facet1->InsertVertex(addvertex);
					obj.AddFacet(facet1);
					facet2->InsertVertex(ptrB); facet2->InsertVertex(ptrC); facet2->InsertVertex(addvertex);
					obj.AddFacet(facet2);
					//	���¿׶���
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