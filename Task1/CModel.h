#pragma once

namespace ZR
{
	class CModel
	{
	public:
		CModel();
		~CModel();
	private:


	protected:
		virtual long long GetPoints() = 0;
		virtual long long GetNearArea() = 0;
		virtual long GetNumbersOfFacets() = 0;
	};

}


