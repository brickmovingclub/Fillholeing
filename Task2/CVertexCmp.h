#pragma once

namespace ZR
{
	class CVertex;
	class CVertexCmp
	{
	public:/*
		bool operator()(const CVertex &v1,const CVertex &v2)const
		{
			return ((v1.GetX() < v2.GetX()) || (v1.GetX() == v2.GetX() && v1.GetY() < v2.GetY()) || (v1.GetX() == v2.GetX() && v1.GetY() == v2.GetY() && v1.GetZ() < v2.GetZ()));
		}
		*/
		bool operator()(std::shared_ptr<CVertex> ptr1, std::shared_ptr<CVertex> ptr2)const;
	};

}


