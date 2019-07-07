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
		~CVector();

		//	����
		float GetX()const;
		float GetY()const;
		float GetZ()const;

		void SetVector(const float &x, const float &y, const float &z);

		//	����
		CVector operator *(const CVector &vector);				//	�����Ĳ��
		void  operator =(const CVector &vector);
	private:
		float _x, _y, _z;
	};


}


