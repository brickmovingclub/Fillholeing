#pragma once
namespace ZR
{

	//	����
	class CVector
	{
	public:
		CVector();
		CVector(const CVector &vector);
		CVector(const float &x,const float &y,const float &z);
		CVector(std::shared_ptr<CVertex> &vertexA, std::shared_ptr<CVertex> &vertexB);
		~CVector();

		//	����
		float GetX()const;
		float GetY()const;
		float GetZ()const;

		void SetVector(const float &x, const float &y, const float &z);

		//	����
		CVector operator *(const CVector &vector);				//	�����Ĳ��
		void  operator =(const CVector &vector);

		// ���������
		static double DotProduct(const CVector &a, const CVector &b);
		//������ģ
		static double NormOfVector(const CVector &a);
	private:
		float _x, _y, _z;
	};


}


