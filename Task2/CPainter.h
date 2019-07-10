#pragma once

namespace ZR
{
	class CModel;
	class CPainter
	{
	public:
		CPainter();
		~CPainter();
		static CPainter *Instance();			//	��ȡ����ʵ��
		void SetModel(CModel &model);			//	������ָ��ģ��
		void Draw();
		void Flush();
		void Colorful();						//	��ָ���������ڵ���ɫ
		void SetPrams(const unsigned long &num, const unsigned int num2);
	private:
		CModel *_model;				//	ģ��
		unsigned long _num1 = 0;		//	ָ��������
		unsigned int _num2 = 0;			//	ָ���������
	};
}


