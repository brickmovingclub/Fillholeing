
#include "stdafx.h"
#include "Common.h""

#include "CVertex.h"
#include "CFacet.h"
#include "CVertexCmp.h"
#include "CModel.h"
#include "CVector.h"

#include "CReader.h"

namespace ZR
{
	CReader::CReader()
	{
	}

	CReader::~CReader()
	{
	}

	size_t CReader::GetFileSize()
	{
		ifstream file;
		size_t fileSize;
		//file.open(_fileDir.c_str(), ios::binary | ios::in);
		if (!file)
		{
			//cout << "Error: file \"" << _fileDir.c_str() << "\" do not exist!" << endl;    // ʧ��
			fileSize = 0;
		}
		else
		{
			file.seekg(0, ios::end);
			fileSize = file.tellg();
		}
		return fileSize;
	}
	void CReader::ReadAscallPly(const fs::path &fileDir, CModel &model)
	{
		string name, useless;
		unsigned long facetNum, vertexNum;
		char *buffer = nullptr;
		float x = 0, y = 0, z = 0;
		unsigned long num1, num2, num3, vertexSize;
		int num = 0, temp;
		
		//	��ȡ�ļ�����
		ReadBuffer(fileDir, &buffer);
		stringstream ss(buffer);
		ss.precision(std::numeric_limits<long double>::digits10);

		//	��ȡ������Ƭ�����Ͷ������
		getline(ss, name);
		getline(ss, name);
		getline(ss, name);
		ss >> useless >> name >> vertexNum >> useless;
		getline(ss, name);
		getline(ss, name);
		getline(ss, name);
		ss >> useless >> name >> facetNum >> useless;
		getline(ss, name);
		getline(ss, name);

		std::vector<std::shared_ptr< CVertex>> allPoints;				//	��ply�ļ����еĵ���ʱ�洢�������������ת��Ϊstl�ļ�
		//std::vector<std::shared_ptr< CVertex>>::iterator iter = allPoints.begin();
		//allPoints.resize(vertexNum + 1);
		for (int i = 0; i < vertexNum; ++i)
		{
			std::shared_ptr< CVertex> vertex(new CVertex());
			allPoints.push_back(vertex);
		}

		//std::cout << "vector.size():" << allPoints.size() << std::endl;
		//	��ȡ���еĶ���
		while (vertexNum--)
		{
			ss >> x >> y >> z ;
			//std::shared_ptr< CVertex> ptr = allPoints[0];
		//	std::cout << ptr->GetX() << std::endl;
			allPoints[num]->SetVertex(x,y,z,num);
			model.AddPoints(allPoints[num], num);
			++num;
		} 
	//	std::cout << "x:" << x << "y:" << y << "z:" << z << std::endl;

		//	��ȡ���е���Ƭ
		num = 0;
		while (facetNum--)
		{
			ss >> vertexSize >> num1 >> num2 >> num3;
			std::shared_ptr<CFacet> oneFace(new CFacet);
			oneFace->SetNumber(num++);
			oneFace->InsertVertex(allPoints[num1]);
			oneFace->InsertVertex(allPoints[num2]);
			oneFace->InsertVertex(allPoints[num3]);

			model.AddFacet(oneFace);

		}

		free(buffer);
		allPoints.clear();
	}

	void CReader::ReadAscallStl(const fs::path &fileDir, CModel &model)
	{


		char *buffer = nullptr;				//	��ʱ�洢�ļ�����
		int i;
		long double x, y, z;				//	���ÿһ��������Ƭ���������㣨�����Ⱥ�˳��
		unsigned long facetNum = 0;			//	ģ�͵�������Ƭ��ţ����ϵ���
		string name, useless;				//	��Ч���ݣ��ļ���ͷ���Լ�һЩ������Ϣ��

		//	��ȡ�ļ�����
		ReadBuffer(fileDir, &buffer);
		stringstream ss(buffer);
		ss.precision(std::numeric_limits<long double>::digits10);		//	���ö�ȡdouble�������ݵĽ��ȣ�stringstreamĬ�Ͼ���Ϊ6

		// ȥ��������������
		getline(ss, name);
		//ss.get();
		do {
			ss >> useless;
			if (useless != "facet")
				break;
			getline(ss, useless);
			getline(ss, useless);

			std::shared_ptr<CFacet> oneFace(new CFacet);
			oneFace->SetNumber(facetNum++);
			for (i = 0; i < 3; i++)
			{
			//	��Ӷ���
				ss >> useless >> x >> y >> z;
				std::shared_ptr<CVertex> vertex(new CVertex(x, y, z, 0));
				model.AddPoints(vertex, oneFace);
			}
			//	���������Ƭ
			model.AddFacet(oneFace);
			
			getline(ss, useless);
			getline(ss, useless);
			getline(ss, useless);
		} while (1);

		free(buffer);				

		//	�����ж�����б��
		model.SerialVertex();
	}

	void CReader::ReadBinaryStl(const fs::path &fileDir, CModel &model)
	{

	}

	void CReader::TransformToPly(CModel &model, const fs::path &plyFileName)
	{
		long vertexNum, facetNum;

		//	���ply�ļ����������򴴽�
		ofstream fout(plyFileName.c_str(), fstream::out);
		fout.precision(std::numeric_limits<double>::digits10);

		//	��ȡ��Ƭ���Ͷ�����
		vertexNum = model.GetNumOfVertes();
		facetNum = model.GetnumberOfFacets();
		if (fout.is_open())
		{
			fout << "ply" << endl;
			fout << "format ascii 1.0" << endl;
			fout << "comment created by platoply" << endl;
			fout << "element vertex " << vertexNum << endl;
			fout << "property float32 x" << endl;
			fout << "property float32 y" << endl;
			fout << "property float32 z" << endl;
			fout << "element face " << facetNum << endl;
			fout << "property list uint8 int32 vertex_indices\nend_header" << endl;

			model.GetData(fout);
			fout.close();
		}
	}
	void CReader::TransformToStl(CModel &model, const fs::path &stlFileName)
	{
		std::shared_ptr< CVertex> ptrVertex1, ptrVertex2, ptrVertex3;
		CVector vector1, vector2,mormalVector;									//	������ʱ�������������㷨����
		ofstream fout(stlFileName.c_str(), fstream::out);
		fout.precision(std::numeric_limits<double>::digits10);
		if (fout.is_open())
		{
			fout << "solid bunny" << endl;
			for (auto iter : model._listFacets)
			{
				ptrVertex1 = iter->GetVertex(0);
				ptrVertex2 = iter->GetVertex(1);
				ptrVertex3 = iter->GetVertex(2);
				//	���㷨����
				vector1.SetVector(ptrVertex2->GetX() - ptrVertex1->GetX(), ptrVertex2->GetY() - ptrVertex1->GetY(), ptrVertex2->GetZ() - ptrVertex1->GetZ());
				vector2.SetVector(ptrVertex3->GetX() - ptrVertex2->GetX(), ptrVertex3->GetY() - ptrVertex2->GetY(), ptrVertex3->GetZ() - ptrVertex2->GetZ());
				mormalVector = vector1 * vector2;

				fout << "facet normal " << mormalVector.GetX() << " " << mormalVector.GetY() << " " << mormalVector.GetZ() << endl;
				fout << "outer loop" << endl;
				fout << "vertex " << ptrVertex1->GetX()<<" "<< ptrVertex1->GetY() <<" "<< ptrVertex1->GetZ() << endl;
				fout << "vertex " << ptrVertex2->GetX() <<" "<< ptrVertex2->GetY() << " "<<ptrVertex2->GetZ() << endl;
				fout << "vertex " << ptrVertex3->GetX() <<" "<< ptrVertex3->GetY() <<" "<< ptrVertex3->GetZ() << endl;

				//fout << "vertex " << iter->GetVertex[1]->GetX() << iter->GetVertex[1]->GetY() << iter->GetVertex[1]->GetZ() << endl;
				//fout << "vertex " << iter->GetVertex[2]->GetX() << iter->GetVertex[2]->GetY() << iter->GetVertex[2]->GetZ() << endl;
				fout << "endloop" << endl;
				fout << "endfacet" << endl;
			}		
			fout << "endsolid bunny" << endl;
			fout.close();
		}
	}

	void CReader::ReadBuffer(const fs::path &fileDir, char **buffer)
	{
		FILE * pFile;
		long lSize;
		size_t result;

		string fileName = fileDir.string();
		fopen_s(&pFile, fileName.c_str(), "rb");
		if (pFile == NULL)
		{
			fputs("File error", stderr);
			exit(1);
		}

		/* ��ȡ�ļ���С */
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		/* �����ڴ�洢�����ļ� */
		*buffer = (char*)malloc(sizeof(char)*lSize);
		if (*buffer == NULL)
		{
			return;
		}

		result = fread(*buffer, 1, lSize, pFile);
		if (result != lSize)
		{
			return;
		}
		fclose(pFile);
	}

}


