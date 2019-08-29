
#include "stdafx.h"

#include "CVertex.h"
#include "CFacet.h"

#include "CVector.h"

#include "Common.h""
namespace ZR
{

	Common::Common()
	{

	}
	Common::~Common()
	{

	}

	int Common::CalculateTimeDiff(const std::chrono::system_clock::time_point &startTime, const std::chrono::system_clock::time_point endTime)
	{
		auto usingTime = endTime - startTime;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(usingTime);
		return ms.count();
	}
	/*
	int Common::CalculateTimeDiff(const time_t &startTime, const time_t &endTime)
	{
		return (double)(startTime - endTime) / CLOCKS_PER_SEC;
	}
	*/
	bool Common::VertexIsValid(std::shared_ptr<CVertex> &vertexA, std::shared_ptr<CVertex> &vertexB, std::shared_ptr<CVertex> &vertexP, std::list< std::shared_ptr<CVertex>> &holes)
	{
		//新增点的合法性验证(点vertexA、vertexB、vertexP 是组成三角形的三个顶点，vertexP 是添加点)
  //合法：true  不合法：false
		CVector vec_AP(vertexP->GetX() - vertexA->GetX(), vertexP->GetY() - vertexA->GetY(), vertexP->GetZ() - vertexA->GetZ());
		CVector vec_AB(vertexB->GetX() - vertexA->GetX(), vertexB->GetY() - vertexA->GetY(), vertexB->GetZ() - vertexA->GetZ());

		CVector n = vec_AP * vec_AB; // 确定单位法向量

		//点A,B,P共线
		double angle = CVector::NormOfVector(n);
		if (angle == 0)
			return false;

		//判断P点在洞内还是洞外
		std::list<std::shared_ptr<CVertex>>::iterator it = holes.begin();
		std::list<std::shared_ptr<CVertex>>::iterator its = it;
		its++;
		while (it != holes.end())
		{
			if (its == holes.end())
				its = holes.begin();
			std::shared_ptr<CVertex> vertexM = *it;
			std::shared_ptr<CVertex> vertexN = *its;
			if (*it != vertexA)
			{
				CVector vec_MP(vertexP->GetX() - vertexM->GetX(), vertexP->GetY() - vertexM->GetY(), vertexP->GetZ() - vertexM->GetZ());
				CVector vec_MN(vertexN->GetX() - vertexM->GetX(), vertexN->GetY() - vertexM->GetY(), vertexN->GetZ() - vertexM->GetZ());
				CVector m = vec_MP * vec_MN;

				double angle_m_n_cos = CVector::DotProduct(m, n) / (CVector::NormOfVector(m) * CVector::NormOfVector(n));
				if (angle_m_n_cos < 0)
					return false;
			}
			it++;
			its++;
		}
		return true;
	}
	void  Common::CalTriangleCricumcenter(std::shared_ptr< CVertex> v1, std::shared_ptr< CVertex> v2, std::shared_ptr< CVertex> v3, std::shared_ptr< CVertex> center)
	{
		//	e1 = v3 - v2;e2 = v1 - v3; e3 = v2 - v1;
		//	d1 = e2.e3;d2 = -e3.e1;d3 = -e1.e2;
		//	c1 = d2d3;c2 = d3d1;c3 = d1d2;
		//c = c1 + c2 + c3;
		// 外心坐标： o = (c2 +c3)v1 + (c3+c1)v2 +(c1+c2)v3/2c;
		CVector e1, e2, e3;
		double d1, d2, d3, c;
		double c1, c2, c3;
		e1.SetVector(v3->GetX() - v2->GetX(), v3->GetY() - v2->GetY(), v3->GetZ() - v2->GetZ());
		e2.SetVector(v1->GetX() - v3->GetX(), v1->GetY() - v3->GetY(), v1->GetZ() - v3->GetZ());
		e3.SetVector(v2->GetX() - v1->GetX(), v2->GetY() - v1->GetY(), v2->GetZ() - v1->GetZ());

		d1 = -(e2.GetX() *e3.GetX() + e2.GetY() *e3.GetY() + e2.GetZ() *e3.GetZ());
		d2 = -(e1.GetX() *e3.GetX() + e1.GetY() *e3.GetY() + e1.GetZ() *e3.GetZ());
		d3 = -(e1.GetX() *e2.GetX() + e1.GetY() *e2.GetY() + e1.GetZ() *e2.GetZ());

		c1 = d2 * d3;
		c2 = d3 * d1;
		c3 = d1 * d2;

		c = c1 + c2 + c3;
		center->SetVertex(((c2 + c3)* v1->GetX() + (c3 + c1) * v2->GetX() + (c1 + c2) * v3->GetX())/ (2 * c),
			((c2 + c3)* v1->GetY() + (c3 + c1) * v2->GetY() + (c1 + c2) * v3->GetY())/ (2 * c),
			((c2 + c3)* v1->GetZ() + (c3 + c1) * v2->GetZ() + (c1 + c2) * v3->GetZ())/ (2 * c),
			0);
		//center->SetVertex((c3 + c2) / (2 * c), (c1 + c3) / (2 * c), (c1 + c2) / (2 * c), 0);
	}

	std::shared_ptr< CFacet> Common::Calangel( std::list < std::shared_ptr< CVertex>> &holes)
	{
		// 返回当前孔洞中三角形夹角最小值
		float angleResult = 0,gngleTemp = 0;
		std::shared_ptr< CVertex > vertex1, vertex2;
		std::map<float, std::shared_ptr< CFacet>> angleFacetMap;
		std::list<std::shared_ptr< CVertex>>::iterator it = holes.begin(),move;
		std::shared_ptr< CVertex > ptrA, ptrB, ptrC;

		
		while(1)
		{
			move = it++;
			ptrA = *move++;
			ptrB = *move++;
			if (move != holes.end())
				ptrC = *move;
			else
				break;

			std::list<std::shared_ptr< CFacet>> listFace = ptrB->GetNearFacets();
			for (auto iter : listFace)
			{
				int i = iter->GetVertexNum(ptrB);
				vertex1 = iter->GetVertex((i + 1 + 3) % 3);
				vertex2 = iter->GetVertex((i - 1 + 3) % 3);

				CVector vector1(ptrB, vertex1);
				CVector vector2(ptrB, vertex2);


				double a1, a2, ab, cosr;
				a1 = sqrt(vector1.GetX() * vector1.GetX() + vector1.GetY() * vector1.GetY() + vector1.GetZ() *vector1.GetZ());
				a2 = sqrt(vector2.GetX() * vector2.GetX() + vector2.GetY() * vector2.GetY() + vector2.GetZ() *vector2.GetZ());
				ab = vector1.GetX() * vector2.GetX() + vector1.GetY() * vector2.GetY() + vector1.GetZ() *vector2.GetZ();

				cosr = ab / (a1 *a2);
				gngleTemp += (acos(cosr) * 180 / PI);
			}
			std::shared_ptr< CFacet> facet = std::make_shared<CFacet>();
			facet->InsertVertex(ptrA); facet->InsertVertex(ptrB); facet->InsertVertex(ptrC);
			angleFacetMap.insert(std::make_pair((360 - gngleTemp), facet));
		}
		
		//	更新孔洞
		std::shared_ptr< CFacet> facet = angleFacetMap.begin()->second;
		holes.erase(std::find(holes.begin(), holes.end(), facet->GetVertex(1)));
		return facet;
		//std::shared_ptr< CFacet> facet = *listFace.begin();

		////	计算向量 BA 与BC之间的夹角(用点B的领域三角面片来估计孔洞的法向量方向)
	
		//
		//std::shared_ptr<CVertex> vertex0 = facet->GetVertex(0);
		//std::shared_ptr<CVertex> vertex1 = facet->GetVertex(1);
		//std::shared_ptr<CVertex> vertex2 = facet->GetVertex(2);

		//CVector vector1(ptrB, ptrA);
		//CVector vector2(ptrB, ptrC);

		//CVector normal1 = vector1 * vector2;

		//double a1, a2, ab, cosr; float angle;
		//a1 = sqrt(vector1.GetX() * vector1.GetX() + vector1.GetY() * vector1.GetY() + vector1.GetZ() *vector1.GetZ());
		//a2 = sqrt(vector2.GetX() * vector2.GetX() + vector2.GetY() * vector2.GetY() + vector2.GetZ() *vector2.GetZ());
		//ab = vector1.GetX() * vector2.GetX() + vector1.GetY() * vector2.GetY() + vector1.GetZ() *vector2.GetZ();

		//cosr = ab / (a1 *a2);
		//angle = (acos(cosr) * 180 / PI);


		////	计算当前领域三角形的法向量
		//char num;
		//std::shared_ptr< CFacet> currentFacet = ptrB->GetFaceNumByVertex(ptrB, ptrA, num);

		//std::shared_ptr<CVertex> vertexfirst = currentFacet->GetVertex(0);
		//std::shared_ptr<CVertex> vertexsecond = currentFacet->GetVertex(1);
		//std::shared_ptr<CVertex> vertexthird = currentFacet->GetVertex(2);
		//std::shared_ptr<CVertex> vertexD = currentFacet->GetVertex(num);

		//CVector v1(ptrB, ptrA);
		//CVector v2(ptrA, vertexD);

		//CVector n2 = v1 * v2;			//	AB X BD

		////	分别计算孔洞的法向量和三角形的法向量
		///*cout << "*******************" << endl;
		//cout << normal1.GetX() << "\t" << normal1.GetY() << "\t" << normal1.GetZ() << endl;
		//cout << "*******************" << endl;*/

		////	检测两法向量是否同向
		//double b1, b2, b12;
		//float angle2;
		//b1 = sqrt(normal1.GetX() * normal1.GetX() + normal1.GetY() * normal1.GetY() + normal1.GetZ() *normal1.GetZ());
		//b2 = sqrt(n2.GetX() * n2.GetX() + n2.GetY() * n2.GetY() + n2.GetZ() *n2.GetZ());
		//b12 = normal1.GetX() * n2.GetX() + normal1.GetY() * n2.GetY() + normal1.GetZ() *n2.GetZ();

		//angle2 = (acos(b12 / (b1 * b2)) * 180 / PI);
		//if (0 <= angle2 && angle2 < 90)		//	两向量同向
		//	angle = (acos(cosr) * 180 / PI);
		//else if (angle2 >= 90 && angle2 <=180)
		//	angle = 360 - (acos(cosr) * 180 / PI);
		//else
		//	angle = 0;

		//cout << "*************夹角*******************" << endl;
		//cout << angle << endl;
		//cout << endl;
		//	类型判断
	/*	if (0 < angleResult && angleResult <= 90)
			return LESS90;
		else if (angleResult > 90 && angleResult <= 180)
			return LESS180ANDBIGGER90;
		else if (angleResult > 180 && angleResult < 360)
			return BIGGER180;
		else
			return  -1;*/
	}
}

