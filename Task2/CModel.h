#pragma once

namespace ZR
{
	enum VectorType
	{
		VECTOR = 1,						//	ģ�����е�Ĵ洢����ѡ��
		SET,
		MAP
	};
	//	ģ��
	class CVertex;
	class CVertexCmp;
	class CFacet;
	class CModel
	{
	public: 
		CModel();
		~CModel();

		friend class CReader;
		friend class CPainter;
		void AddPoints(std::shared_ptr<CVertex> &vertex, const size_t &num);
		void AddPoints(std::shared_ptr<CVertex> &vertex, const std::shared_ptr< CFacet> &facet);						//	��Ӷ���
		void AddFacet(const std::shared_ptr< CFacet> &facet);							//	ģ������������Ƭ
		void SerialVertex();															//	Ϊmap�еĶ����ţ��������Ϊ��stl�ļ�תply�ļ�
		void ReadBuffer(fs::path &fileDir, char **buffer);															//	��ȡstl�ļ�����δbuffer�����ڴ�
		void Calculate(fs::path fileDir,int containType);							//	��ȡstl�ļ�������
		void Calculate2(fs::path fileDir, int containType);

		void SetOppositeEdge(const std::shared_ptr< CFacet> &facet,int &num);														//	Ϊÿһ������Ƭ������ÿ�����������������Ƭ
		void WriteToPlyFile(fs::path  fileName, int containType);					//��stl�ļ�ת��Ϊply�ļ�
		size_t GetTerritory(const int &pointNum, const int &n);									//	��ȡָ�����n������
		size_t GetTerritory(double x,double y,double z, int n);									//	��ȡָ�����n������		
		size_t GetTerritory(const std::shared_ptr< CVertex> &vertex,const int &n);									//	��ȡָ�����n������
		void GetTerritory(const std::shared_ptr< CVertex> &vertex, const int &n, std::set<std::shared_ptr<CVertex>, CVertexCmp> &result);


		size_t GetPoints(int containType);											// ��ȡģ�͵��ܵĶ�����
		size_t GetNumOfVertes();													//	��ȡģ�͵Ķ������
		//std::shared_ptr< CVertex> GetVertex(size_t i);								//	��ȡmap������ָ���Ķ���
		size_t GetnumberOfFacets();													//	��ȡģ�͵���Ƭ��
		int GetHoles();																//	��ȡ��ǰģ�͵Ŀ׶���
		void GetHoles2(std::vector<std::list<std::shared_ptr< CVertex>>> & result);																//	��ȡ��ǰģ�͵Ŀ׶���

		void test();
		float CalculateSumArea();											//	����ģ�͵������
		float CalculateVolume();									//	�������
		void GetData(ofstream &fout);										//	��ȡģ�����еĶ������Ƭ
	private:
		CVertexCmp compareMethod;
		std::map< std::shared_ptr<CVertex>, long long, CVertexCmp> _mapModelPoints;	//	���ģ�͵����ж��㣨û���ظ�����ڣ�
		std::vector< std::shared_ptr<CVertex>> _vectorModelPoints;					//	
		std::set< std::shared_ptr<CVertex>, CVertexCmp> _setModelPoints;

		std::list< std::shared_ptr<CFacet>> _listFacets;						//	���ģ�͵�������Ƭ
		

		//void ReadFile();
	};
}


