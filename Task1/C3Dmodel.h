#pragma once
#include "CModel.h"


namespace ZR
{

	class C3Dmodel :
		public CModel
	{
	public:
		C3Dmodel(fs::path fileFullPath);
		virtual ~C3Dmodel();
	public:
		virtual long long GetPoints();
		virtual long long GetNearArea();
		virtual long GetNumbersOfFacets();
		void Calculate();
		void Calculate2();

	private:
		//std::vector<std::tuple<string, string, string>> _vertexs;
		std::unordered_map<string, std::pair<string, string>> _vertexs;			// 插入更多的数据，牺牲存储空间，来实现求每一个节点的领域
		std::vector<std::tuple<float, float,float>> _vectorPoints;		
		//std::unordered_map<string, string, string> _vertexs;
		long long _nearArea;									//	领域数量
		long _numberOfFacets;									//	切面数量
		fs::path _filePath;										//	stl文件路徑
	};


}

