#pragma once

namespace ZR
{
	//	��
	class CFacet;
	class CVertexCmp;
	class CVertex
{
public:
	explicit CVertex(long double x, long double y, long double z, size_t number);
	CVertex(const CVertex &vertex);
	CVertex();
	~CVertex();

	void AddFaces(const std::shared_ptr< CFacet> & facet);
	void AddNextVertex(const std::shared_ptr< CVertex> & vertex);

	void GetNearPoints(std::set<std::shared_ptr<CVertex>, CVertexCmp> &setPoints);

	void GetNextVertex(std::list < std::shared_ptr< CVertex>> &listback);						//	��ȡ��ǰ������ָ�������ڵ㣨�������ڵ㱾��

	std::list<std::shared_ptr< CFacet>> & GetNearFacets();
	std::shared_ptr< CFacet> GetFaceNumByVertex(const std::shared_ptr< CVertex> &vertex1, const std::shared_ptr< CVertex> &vertex2,char &num);		//	ͨ�����ȡ��Ƭ���,��Ϊ���ҵ�����Ƭ���ù���ߵ���Ƭ��
	 
	int GetFacetNum()const;

	long double GetX() const {
		return _x;
	}
	long double GetY() const {
		return _y;
	}
	long double GetZ() const {
		return _z;
	}
	size_t GetNum() const {
		return _serialNumber;
	}
	void SetVertex(long double x, long double y, long double z, size_t number) {
		_x = x;
		_y = y;
		_z = z;
		_serialNumber = number;
	}
	void SetNumber(size_t num)
	{
		_serialNumber = num;
	}

	
	bool IsBorderVertex()				//	�жϵ�ǰ�����Ƿ������ڱ߽��
	{
		//return ((_belongFacets.size() == _nextVertexs.size()) ? true : false);
		int num1 = _belongFacets.size();
		int num2 = _nextVertexs.size();
		return (num1 != num2) ? true : false;
	}
	bool operator < (const CVertex & vertex)
	{
		return ((this->GetX() < vertex.GetX()) || (this->GetX() == vertex.GetX() && this->GetY() < vertex.GetY()) || (this->GetX() == vertex.GetX() && this->GetY() == vertex.GetY() && this->GetZ() < vertex.GetZ()));
	}
	void operator =(const CVertex &vertex1)
	{
		this->SetVertex(vertex1.GetX(), vertex1.GetY(), vertex1.GetZ(), vertex1.GetNum());
	}
	bool  operator == (const CVertex &vertex)
	{
		return (vertex.GetX() == this->GetX() && vertex.GetY() == this->GetY() && vertex.GetZ() == this->GetZ());
	}
	bool  operator == (CVertex &vertex)
	{
		return (vertex.GetX() == this->GetX() && vertex.GetY() == this->GetY() && vertex.GetZ() == this->GetZ());
	}
private:
	long double _x;						//	�����x����
	long double _y;						//	�����y����
	long double _z;						//	�����z����

	std::list<std::shared_ptr< CFacet>> _belongFacets;			//����������Ƭ
	size_t _serialNumber;										//	�������к�
	//std::shared_ptr<CVertex> _next;							//	��ǰ����ָ����һ������
	std::list < std::shared_ptr< CVertex>> _nextVertexs;			//	��ǰ������ָ���������һ������
	
	
};

}

