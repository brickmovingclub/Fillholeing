#pragma once

namespace ZR
{
	//	面片
	#define POINTS	3
	class CVertex;
	class CFacet
	{
	public:
		CFacet();
		~CFacet();
		void InsertVertex(const std::shared_ptr<CVertex> &vertex);
		void SetOppositeHEdge(const std::pair<std::shared_ptr<CFacet>, char> &oppsitePair);					//	设置共用三角面片的边的三角行序号（前30位为序号，后两位为当前面片的顶点编号）
		size_t GetvertexNums() {
			return _listFacetPoints.size();
		}

		bool IsBorder();													//	检测当前面片是否是孔洞边缘
		void GetBorderVertex(std::shared_ptr< CVertex> & vStart,std::shared_ptr< CVertex> & vEnd);	//	获取孔洞边缘的两个连续的顶点（vStart-》vEnd）,返回值表示当前使用的是第几条边
		bool IsExist(const std::shared_ptr< CVertex> &vertex);				//	检测当前点是否存在于该面片中
		bool IsExist(const std::shared_ptr< CVertex> &vertex1, const std::shared_ptr< CVertex> &vertex2,char &numback);
		std::shared_ptr<CVertex> GetVertex(const int &i);
		void SetNumber(const unsigned long &num);							//	设置面片序号
		unsigned long GetFacetNum()const;									//	获取面片序号


	private:
		std::vector<std::shared_ptr<CVertex>> _listFacetPoints;				//	组成面片的n个点的编号，包含先后顺序,且连续的两个点组成一条边（有方向）
		//unsigned long _oppositeHEdge[POINTS] = {0};							//	_listFacetPoints 中每条边所对应的反向边（前30位是面片编号，后两位是对应面片中的边的序号，也就是采用的半边编码数据结构）
		std::vector<std::pair<std::shared_ptr<CFacet>,char>> _oppositeHEdge;	// 三条边所对应的共享三角片面
		//std::list<CVertex> _narmalVector;									//	面片的法向量
		unsigned long _facetNumber;											//	面片编号
	};

}


