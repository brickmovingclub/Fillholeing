#pragma once

#define BLOCKSIZE 1024*1024*1024*256			// 一次性能读取的最大block
namespace ZR
{
	class CModel;
	class CReader
	{
	public:
		CReader();
		~CReader();
		
		void ReadBuffer(const fs::path &fileDir, char **buffer);										//	读取stl文件的内容，并将其存储到buffer中
		void ReadAscallStl(const fs::path &fileDir, CModel &model);										//	为模型读取Ascall码文件数据（STL 文件和ascall码文件的头部信息均为asall码，且信息保持一致）
		void ReadAscallPly(const fs::path &fileDir, CModel &model);										//	读Ascall码的ply文件
		void ReadBinaryStl(const fs::path &fileDir, CModel &model);										//	为模型读取二进制码文件数据

		void TransformToPly(CModel &model, const fs::path &plyFileName);					//	将stl文件转换为ply文件
		void TransformToStl(CModel &model, const fs::path &stlFileName);					//	将ply文件转换为stl文件
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
