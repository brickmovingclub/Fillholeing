
#include "stdafx.h"

#include "CVertexCmp.h"
#include "CFacet.h"
#include "CVertex.h"


namespace ZR
{
	CVertex::CVertex(double x, double y, double z, size_t number):_x(x),_y(y),_z(z), _serialNumber(number)
	{
	}
	CVertex::CVertex():_x(0), _y(0), _z(0), _serialNumber(0)
	{
	}


	CVertex::~CVertex()
	{
		_belongFacets.clear();
		_nextVertexs.clear();

	}

	CVertex::CVertex(const CVertex &vertex)
	{
		this->SetVertex(vertex.GetX(), vertex.GetY(), vertex.GetZ(), vertex.GetNum());
	}
	void CVertex::GetNearPoints(std::set<std::shared_ptr<CVertex>, CVertexCmp> &setPoints) {

		//std::cout << " GetNearPoints" << std::endl;
		for (auto iter : _belongFacets)
		{
			for (int i = 0; i < iter->GetvertexNums(); ++i)
			{
				std::shared_ptr< CVertex> ptr = iter->GetVertex(i);
				long num = ptr->GetNum();
				setPoints.insert(iter->GetVertex(i));
				//if (this->GetNum() == iter->GetVertex(i)->GetNum())
			//	{
				//	setPoints.insert(iter->GetVertex((i + 1) % 3));
				//	break;
				//}
			}
		}
		//std::cout << " 当前set大小:" << setPoints.size() << std::endl;
	}

	void CVertex::GetNearPoints(std::set<std::shared_ptr<CVertex>, CVertexCmp> &nextVertexs, std::set<std::shared_ptr<CVertex>, CVertexCmp> &resultVertexs)
	{
		for (auto iter : _belongFacets)
		{
			for (int i = 0; i < iter->GetvertexNums(); ++i)
			{
				if (resultVertexs.insert(iter->GetVertex(i)).second)
				{
					nextVertexs.insert(iter->GetVertex(i));
				}				
			}
		}
	}

	void CVertex::GetNextVertex(std::list < std::shared_ptr< CVertex>> &listback)
	{
		listback.clear();
		std::copy(_nextVertexs.begin(), _nextVertexs.end(), back_inserter(listback));				
	}
	std::list<std::shared_ptr< CFacet>> & CVertex::GetNearFacets()
	{
		return _belongFacets;
	}
	std::shared_ptr< CFacet> CVertex::GetFaceNumByVertex(const std::shared_ptr< CVertex> &vertex1, const std::shared_ptr< CVertex> &vertex2,char &num)
	{
		for (auto iter : _belongFacets)
		{
			if (iter->IsExist(vertex1, vertex2, num))
			{
				return iter;
			}
		}
		return nullptr;

	}

	int CVertex::GetFacetNum()const
	{
		return _belongFacets.size();
	}
	void CVertex::AddFaces(const std::shared_ptr< CFacet> & facet) {
		_belongFacets.push_back(facet);
	}
	void CVertex::AddNextVertex(const std::shared_ptr< CVertex> &vertex)
	{
		_nextVertexs.push_back(vertex);
	}

}

