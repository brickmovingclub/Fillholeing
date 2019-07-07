#pragma once

namespace ZR
{
	//	点
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

	void GetNextVertex(std::list < std::shared_ptr< CVertex>> &listback);						//	获取当前顶点所指向的领域节点（不包括节点本身）

	std::list<std::shared_ptr< CFacet>> & GetNearFacets();
	std::shared_ptr< CFacet> GetFaceNumByVertex(const std::shared_ptr< CVertex> &vertex1, const std::shared_ptr< CVertex> &vertex2,char &num);		//	通过点获取面片序号,并为查找到的面片设置共享边的面片号
	 
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

	
	bool IsBorderVertex()				//	判断当前顶点是否是属于边界边
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
	long double _x;						//	顶点的x坐标
	long double _y;						//	顶点的y坐标
	long double _z;						//	顶点的z坐标

	std::list<std::shared_ptr< CFacet>> _belongFacets;			//点所属的面片
	size_t _serialNumber;										//	顶点序列号
	//std::shared_ptr<CVertex> _next;							//	当前顶点指向下一个顶点
	std::list < std::shared_ptr< CVertex>> _nextVertexs;			//	当前顶点所指向的所有下一个顶点
	
	
};

}

