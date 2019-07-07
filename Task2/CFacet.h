#pragma once

namespace ZR
{
	//	��Ƭ
	#define POINTS	3
	class CVertex;
	class CFacet
	{
	public:
		CFacet();
		~CFacet();
		void InsertVertex(const std::shared_ptr<CVertex> &vertex);
		void SetOppositeHEdge(const std::pair<std::shared_ptr<CFacet>, char> &oppsitePair);					//	���ù���������Ƭ�ıߵ���������ţ�ǰ30λΪ��ţ�����λΪ��ǰ��Ƭ�Ķ����ţ�
		size_t GetvertexNums() {
			return _listFacetPoints.size();
		}

		bool IsBorder();													//	��⵱ǰ��Ƭ�Ƿ��ǿ׶���Ե
		void GetBorderVertex(std::shared_ptr< CVertex> & vStart,std::shared_ptr< CVertex> & vEnd);	//	��ȡ�׶���Ե�����������Ķ��㣨vStart-��vEnd��,����ֵ��ʾ��ǰʹ�õ��ǵڼ�����
		bool IsExist(const std::shared_ptr< CVertex> &vertex);				//	��⵱ǰ���Ƿ�����ڸ���Ƭ��
		bool IsExist(const std::shared_ptr< CVertex> &vertex1, const std::shared_ptr< CVertex> &vertex2,char &numback);
		std::shared_ptr<CVertex> GetVertex(const int &i);
		void SetNumber(const unsigned long &num);							//	������Ƭ���
		unsigned long GetFacetNum()const;									//	��ȡ��Ƭ���


	private:
		std::vector<std::shared_ptr<CVertex>> _listFacetPoints;				//	�����Ƭ��n����ı�ţ������Ⱥ�˳��,�����������������һ���ߣ��з���
		//unsigned long _oppositeHEdge[POINTS] = {0};							//	_listFacetPoints ��ÿ��������Ӧ�ķ���ߣ�ǰ30λ����Ƭ��ţ�����λ�Ƕ�Ӧ��Ƭ�еıߵ���ţ�Ҳ���ǲ��õİ�߱������ݽṹ��
		std::vector<std::pair<std::shared_ptr<CFacet>,char>> _oppositeHEdge;	// ����������Ӧ�Ĺ�������Ƭ��
		//std::list<CVertex> _narmalVector;									//	��Ƭ�ķ�����
		unsigned long _facetNumber;											//	��Ƭ���
	};

}


