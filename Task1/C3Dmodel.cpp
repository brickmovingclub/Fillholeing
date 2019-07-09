


#include "stdafx.h"
#include "CModel.h"
#include "C3Dmodel.h"

namespace ZR
{

	C3Dmodel::C3Dmodel(fs::path fileFullPath):_nearArea(0), _numberOfFacets(0)
	{
		_filePath = fileFullPath;
	}


	C3Dmodel::~C3Dmodel()
	{
		_vertexs.clear();
		_vectorPoints.clear();
	}


	long long C3Dmodel::GetPoints()
	{
		return _vectorPoints.size();
		//return _numberOfFacets + 2;
	}

	long long C3Dmodel::GetNearArea()
	{
		return _nearArea;
	}

	long C3Dmodel::GetNumbersOfFacets()
	{
		/*
		//	假定不会出现重复
		_numberOfFacets = 0;
		ifstream fin(_filePath.c_str());
		while (!fin.eof())
		{
			string strTemp;
			getline(fin, strTemp);
			if (!strTemp.empty())
			{
				_numberOfFacets++;
			}
		}
		fin.close();
		return (_numberOfFacets -2)/7;
		*/
		return _numberOfFacets;
	}

		
	//	使用正则表达式二进制文件读取，太慢了
	void C3Dmodel::Calculate()
	{
		if(fs::exists(_filePath)&&fs::is_regular_file(_filePath))
		{
			std::ifstream fin;
			fin.open(_filePath.c_str(), std::ios::in);
			while (!fin.eof())
			{
				std::string strOneLine;
				getline(fin, strOneLine);
				
				//std::cout << strOneLine << std::endl;
				std::smatch machvertex;
				//	查找面片数
				if (std::regex_match(strOneLine.c_str(), std::regex("[ \t\n]*facet normal.*")))
				{
					_numberOfFacets++;
				}
				//	查找节点
				
				if (std::regex_search(strOneLine.cbegin(), strOneLine.cend(), machvertex, std::regex("[ \t\n]*vertex[ \t\n]*")))
				{
					std::string  strSuffixs = machvertex.suffix().str();
					std::sregex_token_iterator e;
					std::regex sep("[\t\n]*");
					std::sregex_token_iterator pos(strSuffixs.cbegin(), strSuffixs.cend(), sep,-1);
					if (pos != e)
					{
						//string strFirst = *pos;
						//string strSecond = *(++pos);
						//string strThird = *(++pos);
						_vertexs.emplace(*pos++,std::make_pair(*pos++, *pos++));
					}
				}
				
			}
			fin.close();
		}

	}

	static bool cmp(const std::tuple<float,float,float> &a, const std::tuple<float, float, float>& b)
	{
		return std::get<0>(a) > std::get<0>(b); //从大到小排序
	}
	void C3Dmodel::Calculate2()
	{
		FILE * pFile;
		long lSize;
		size_t result;
		char* buffer;
		
		string fileName = _filePath.string();
		fopen_s(&pFile, fileName.c_str(), "rb");
		if (pFile == NULL)
		{
			fputs("File error", stderr);
			exit(1);
		}

		/* 获取文件大小 */
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		/* 分配内存存储整个文件 */
		buffer = (char*)malloc(sizeof(char)*lSize);
		if (buffer == NULL)
		{
			return;
		}

		result = fread(buffer, 1, lSize, pFile);
		if (result != lSize)
		{
			return;
		}
		fclose(pFile);

		_numberOfFacets = 0;
		float x, y, z;
		int i;
		string name, useless;
		stringstream ss(buffer);
		// 去掉首行无用数据
		getline(ss, name);
		ss.get();
		do {
			ss >> useless;
			if (useless != "facet")
				break;
			_numberOfFacets++;
			getline(ss, useless);
			getline(ss, useless);
			for (i = 0; i < 3; i++)
			{
				ss >> useless >> x >> y >> z;
				_vectorPoints.push_back(std::make_tuple(x,y,z));
			}
			getline(ss, useless);
			getline(ss, useless);
			getline(ss, useless);
		} while (1);
	
		sort(_vectorPoints.begin(), _vectorPoints.end(), cmp);
		_vectorPoints.erase(unique(_vectorPoints.begin(), _vectorPoints.end()), _vectorPoints.end());
	
	
	}

}
