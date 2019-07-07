#pragma once

#define BLOCKSIZE 1024*1024*1024*256			// һ�����ܶ�ȡ�����block
namespace ZR
{
	class CModel;
	class CReader
	{
	public:
		CReader();
		~CReader();
		
		void ReadBuffer(const fs::path &fileDir, char **buffer);										//	��ȡstl�ļ������ݣ�������洢��buffer��
		void ReadAscallStl(const fs::path &fileDir, CModel &model);										//	Ϊģ�Ͷ�ȡAscall���ļ����ݣ�STL �ļ���ascall���ļ���ͷ����Ϣ��Ϊasall�룬����Ϣ����һ�£�
		void ReadAscallPly(const fs::path &fileDir, CModel &model);										//	��Ascall���ply�ļ�
		void ReadBinaryStl(const fs::path &fileDir, CModel &model);										//	Ϊģ�Ͷ�ȡ���������ļ�����

		void TransformToPly(CModel &model, const fs::path &plyFileName);					//	��stl�ļ�ת��Ϊply�ļ�
		void TransformToStl(CModel &model, const fs::path &stlFileName);					//	��ply�ļ�ת��Ϊstl�ļ�
		size_t GetFileSize();
	private:
	};

}
/*
ply
format ascii 1.0
comment File exported by Rhinoceros Version 5.0
element vertex 3020
property float x
property float y
property float z
element face 4260
property list uchar uint vertex_index
end_header*/
