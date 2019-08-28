#pragma once

namespace ZR
{

	class CVector;
	class Common
	{
		Common();
		~Common();
	public:
		//	��������ʱ������Ϊ��λ��
		static int CalculateTimeDiff(const std::chrono::system_clock::time_point &startTime, const std::chrono::system_clock::time_point endTime); 

		//static int CalculateTimeDiff(const time_t &startTime, const time_t &endTime);
		static  bool VertexIsValid(std::shared_ptr<CVertex> &vertexA, std::shared_ptr<CVertex> &vertexB, std::shared_ptr<CVertex> &vertexP,  std::list< std::shared_ptr<CVertex>> &holes);
		//	���������ε�����
		static void CalTriangleCricumcenter(std::shared_ptr< CVertex> v1, std::shared_ptr< CVertex> v2, std::shared_ptr< CVertex> v3, std::shared_ptr< CVertex> center);

		//	�������ռ������ļн�
		static int Calangel(std::shared_ptr< CVertex > ptrA, std::shared_ptr< CVertex > ptrB, std::shared_ptr< CVertex > ptr);
		//	���׶��޸�ʱ��ӵ�ĺϷ���
		//static bool CheckValidity(const Point & addPoint, std::vector<CLine> &borderLines);
	};
}