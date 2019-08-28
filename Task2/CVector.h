#pragma once
namespace ZR
{

	//	向量
	class CVector
	{
	public:
		CVector();
		CVector(const CVector &vector);
		CVector(const float &x,const float &y,const float &z);
		CVector(std::shared_ptr<CVertex> &vertexA, std::shared_ptr<CVertex> &vertexB);
		~CVector();

		//	方法
		float GetX()const;
		float GetY()const;
		float GetZ()const;

		void SetVector(const float &x, const float &y, const float &z);

		//	重载
		CVector operator *(const CVector &vector);				//	向量的叉积
		void  operator =(const CVector &vector);

		// 两向量点乘
		static double DotProduct(const CVector &a, const CVector &b);
		//向量的模
		static double NormOfVector(const CVector &a);
	private:
		float _x, _y, _z;
	};


}


