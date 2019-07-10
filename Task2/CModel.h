#pragma once

namespace ZR
{
	enum VectorType
	{
		VECTOR = 1,						//	模型所有点的存储容器选择
		SET,
		MAP
	};
	//	模型
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
		void AddPoints(std::shared_ptr<CVertex> &vertex, const std::shared_ptr< CFacet> &facet);						//	添加顶点
		void AddFacet(const std::shared_ptr< CFacet> &facet);							//	模型新增三角面片
		void SerialVertex();															//	为map中的顶点编号，方便后续为将stl文件转ply文件
		void ReadBuffer(fs::path &fileDir, char **buffer);															//	读取stl文件，并未buffer分配内存
		void Calculate(fs::path fileDir,int containType);							//	读取stl文件并处理
		void Calculate2(fs::path fileDir, int containType);

		void SetOppositeEdge(const std::shared_ptr< CFacet> &facet,int &num);														//	为每一个三角片面设置每条边所共享的三角面片
		void WriteToPlyFile(fs::path  fileName, int containType);					//将stl文件转换为ply文件
		size_t GetTerritory(const int &pointNum, const int &n);									//	获取指定点的n阶领域
		size_t GetTerritory(double x,double y,double z, int n);									//	获取指定点的n阶领域		
		size_t GetTerritory(const std::shared_ptr< CVertex> &vertex,const int &n);									//	获取指定点的n阶领域
		void GetTerritory(const std::shared_ptr< CVertex> &vertex, const int &n, std::set<std::shared_ptr<CVertex>, CVertexCmp> &result);


		size_t GetPoints(int containType);											// 获取模型的总的定点数
		size_t GetNumOfVertes();													//	获取模型的顶点个数
		//std::shared_ptr< CVertex> GetVertex(size_t i);								//	获取map容器中指定的顶点
		size_t GetnumberOfFacets();													//	获取模型的面片数
		int GetHoles();																//	获取当前模型的孔洞数
		void GetHoles2(std::vector<std::list<std::shared_ptr< CVertex>>> & result);																//	获取当前模型的孔洞数

		void test();
		float CalculateSumArea();											//	计算模型的总面积
		float CalculateVolume();									//	计算体积
		void GetData(ofstream &fout);										//	读取模型所有的顶点和面片
	private:
		CVertexCmp compareMethod;
		std::map< std::shared_ptr<CVertex>, long long, CVertexCmp> _mapModelPoints;	//	组成模型的所有顶点（没有重复点存在）
		std::vector< std::shared_ptr<CVertex>> _vectorModelPoints;					//	
		std::set< std::shared_ptr<CVertex>, CVertexCmp> _setModelPoints;

		std::list< std::shared_ptr<CFacet>> _listFacets;						//	组成模型的所有面片
		

		//void ReadFile();
	};
}


