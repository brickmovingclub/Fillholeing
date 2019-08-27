
#include "stdafx.h"

#include "CVertex.h"

#include "CFacet.h"
#include "Common.h""



namespace ZR
{
	CFacet::CFacet():_facetNumber(0)
	{
	}


	CFacet::~CFacet()
	{
		_listFacetPoints.clear();
	}

	void CFacet::InsertVertex(const std::shared_ptr<CVertex> &vertex)
	{
		_listFacetPoints.push_back(vertex);
	}

	void CFacet::SetOppositeHEdge(const std::pair<std::shared_ptr<CFacet>, char> &oppsitePair)
	{
		_oppositeHEdge.push_back(oppsitePair);
	}
	bool CFacet::IsBorder()
	{
		std::cout << "±ß£º" << _oppositeHEdge.size() << std::endl;
		return ((_oppositeHEdge.size() == 0) ? true : false);
	}

	void  CFacet::GetBorderVertex(std::shared_ptr< CVertex> & vStart, std::shared_ptr< CVertex> & vEnd)
	{
		std::cout << "size:" << _oppositeHEdge.size() << std::endl;
		if (_oppositeHEdge.size() != 2)
		{
			vStart = vEnd = nullptr;
			return;
		}
		char num1 = _oppositeHEdge.at(0).second;
		char num2 = _oppositeHEdge.at(1).second;

		if (num1 != 0 && num2 != 0)
		{
			vStart = _listFacetPoints.at(0);
			vEnd = _listFacetPoints.at(1);
		}
		else if (num1 != 1 && num2 != 1)
		{
			vStart = _listFacetPoints.at(1);
			vEnd = _listFacetPoints.at(2);
		}
		else if (num1 != 2 && num2 != 2)
		{
			vStart = _listFacetPoints.at(2);
			vEnd = _listFacetPoints.at(0);
		}
		
	}
	bool CFacet::IsExist(const std::shared_ptr< CVertex> &vertex)
	{
		//if (std::find_if(_listFacetPoints.begin(), _listFacetPoints.end(), CVertexCmp) == _listFacetPoints.end())
		auto iter = std::find_if(_listFacetPoints.begin(), _listFacetPoints.end(), [&](std::shared_ptr<CVertex> ptrVertex) {return ((ptrVertex->GetNum() == vertex->GetNum()) ? true : false); });
		if (iter != _listFacetPoints.end())
			return true;
		else
			return false;
	}
	bool CFacet::IsExist(const std::shared_ptr< CVertex> &vertex1, const std::shared_ptr< CVertex> &vertex2,char &numback)
	{
		int num1 = vertex1->GetNum(), num2 = vertex2->GetNum();
		bool v1 = false, v2 = false, v3 = false;
		if (this->GetVertex(0) == vertex1 || this->GetVertex(0) == vertex2)
		{
			v1 = true;
		}
		if (this->GetVertex(1)== vertex1 || this->GetVertex(1) == vertex2)
		{
			v2 = true;
		}
		if (this->GetVertex(2) == vertex1 || this->GetVertex(2) == vertex2)
		{
			v3 = true;
		}

		if (v1 && v2)
		{
			numback = 0;
			return true;
		}
		else if (v1 && v3)
		{
			numback = 2;
			return true;
		}
		else if (v2 && v3)
		{
			numback = 2;
			return true;
		}
		else
		{
			numback = -1;
			return false;
		}
		
		//return (((this->GetVertex(0)->GetNum() == num1) || (this->GetVertex(1)->GetNum() == num1) || (this->GetVertex(2)->GetNum() == num1)) && ((this->GetVertex(0)->GetNum() == num2) || (this->GetVertex(1)->GetNum() == num2) || (this->GetVertex(2)->GetNum() == num2))) ? true : false;
	}

	std::shared_ptr<CVertex>  CFacet::GetVertex(const int &i)
	{
		//std::shared_ptr<CVertex> result(new CVertex);
		std::shared_ptr<CVertex> result = nullptr;
		if (!_listFacetPoints.empty() && 0 <= i && i < _listFacetPoints.size())
		{			
			result = _listFacetPoints.at(i);
		}
		return result;

	}
	void CFacet::SetNumber(const unsigned long &num)
	{
		_facetNumber = num;
	}

	unsigned long CFacet::GetFacetNum()const
	{
		return _facetNumber;
	}

}
