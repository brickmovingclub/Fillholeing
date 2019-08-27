
#include "stdafx.h"
#include "CVertex.h"

#include "CVertexCmp.h"
#include "Common.h""

namespace ZR
{
	bool CVertexCmp::operator()(std::shared_ptr<CVertex> ptr1, std::shared_ptr<CVertex> ptr2)const
	{
		return ((ptr1->GetX() < ptr2->GetX()) || (ptr1->GetX() == ptr2->GetX() && ptr1->GetY() < ptr2->GetY()) || (ptr1->GetX() == ptr2->GetX() && ptr1->GetY() == ptr2->GetY() && ptr1->GetZ() < ptr2->GetZ()));
	}
}


