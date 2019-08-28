
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
		//������ĺϷ�����֤(��vertexA��vertexB��vertexP ����������ε��������㣬vertexP ����ӵ�)
  //�Ϸ���true  ���Ϸ���false
		CVector vec_AP(vertexP->GetX() - vertexA->GetX(), vertexP->GetY() - vertexA->GetY(), vertexP->GetZ() - vertexA->GetZ());
		CVector vec_AB(vertexB->GetX() - vertexA->GetX(), vertexB->GetY() - vertexA->GetY(), vertexB->GetZ() - vertexA->GetZ());

		CVector n = vec_AP * vec_AB; // ȷ����λ������

		//��A,B,P����
		double angle = CVector::NormOfVector(n);
		if (angle == 0)
			return false;

		//�ж�P���ڶ��ڻ��Ƕ���
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
		// �������꣺ o = (c2 +c3)v1 + (c3+c1)v2 +(c1+c2)v3/2c;
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

	int Common::Calangel(std::shared_ptr< CVertex > ptrA, std::shared_ptr< CVertex > ptrB, std::shared_ptr< CVertex > ptrC)
	{
		std::list<std::shared_ptr< CFacet>> listFace = ptrB->GetNearFacets();
		std::shared_ptr< CFacet> facet = *listFace.begin();

		//	�������� BA ��BC֮��ļн�(�õ�B������������Ƭ�����ƿ׶��ķ���������)
		CVector vector1(ptrB,ptrA);
		CVector vector2(ptrB, ptrC);

		std::shared_ptr<CVertex> vertex0 = facet->GetVertex(0);
		std::shared_ptr<CVertex> vertex1 = facet->GetVertex(1);
		std::shared_ptr<CVertex> vertex2 = facet->GetVertex(2);

		CVector vectorAB(vertex0, vertex1);
		CVector vectorBC(vertex1, vertex2);

		double a1, a2, ab, cosr; float angle;
		a1 = sqrt(vector1.GetX() * vector1.GetX() + vector1.GetY() * vector1.GetY() + vector1.GetZ() *vector1.GetZ());
		a2 = sqrt(vector2.GetX() * vector2.GetX() + vector2.GetY() * vector2.GetY() + vector2.GetZ() *vector2.GetZ());
		ab = vector1.GetX() * vector2.GetX() + vector1.GetY() * vector2.GetY() + vector1.GetZ() *vector2.GetZ();

		cosr = ab / (a1 *a2);
		angle = (acos(cosr) * 180 / PI);
		
		//	�ֱ����׶��ķ������������εķ�����
		CVector normal1 = vectorAB * vectorBC;
		CVector normal2 = vector1 * vector2;

		//	������������Ƿ�ͬ��
		double b1, b2, b12;
		float angle2;
		b1 = sqrt(normal1.GetX() * normal1.GetX() + normal1.GetY() * normal1.GetY() + normal1.GetZ() *normal1.GetZ());
		b2 = sqrt(normal2.GetX() * normal2.GetX() + normal2.GetY() * normal2.GetY() + normal2.GetZ() *normal2.GetZ());
		b12 = normal1.GetX() * normal2.GetX() + normal1.GetY() * normal2.GetY() + normal1.GetZ() *normal2.GetZ();

		angle2 = (acos(b12 / (b1 * b2)) * 180 / PI);
		if (0 <= angle2 && angle2 < 90)		//	������ͬ��
			angle = (acos(cosr) * 180 / PI);
		else if (angle2 >= 90 && angle2 <=180)
			angle = 360 - (acos(cosr) * 180 / PI);
		else
			angle = 0;


		//	�����ж�
		if (0 < angle && angle <= 90)
			return LESS90;
		else if (angle > 90 && angle <= 180)
			return LESS180ANDBIGGER90;
		else if (angle > 180 && angle < 360)
			return BIGGER180;
		else
			return  -1;
	}
}

