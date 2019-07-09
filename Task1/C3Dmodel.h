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
		std::unordered_map<string, std::pair<string, string>> _vertexs;			// �����������ݣ������洢�ռ䣬��ʵ����ÿһ���ڵ������
		std::vector<std::tuple<float, float,float>> _vectorPoints;		
		//std::unordered_map<string, string, string> _vertexs;
		long long _nearArea;									//	��������
		long _numberOfFacets;									//	��������
		fs::path _filePath;										//	stl�ļ�·��
	};


}

