#pragma once

namespace ZR
{
	class CModel;
	class CPainter
	{
	public:
		CPainter();
		~CPainter();
		static CPainter *Instance();			//	获取画家实例
		void SetModel(CModel &model);			//	给画家指定模型
		void Draw();
		void Flush();
		void Colorful();						//	给指定点的领域节点上色
		void SetPrams(const unsigned long &num, const unsigned int num2);
	private:
		CModel *_model;				//	模型
		unsigned long _num1 = 0;		//	指定点的序号
		unsigned int _num2 = 0;			//	指定点领域阶
	};
}


