
#include "stdafx.h"
#include "Common.h""

#include "CVertex.h"
#include "CFacet.h"
#include "CVertexCmp.h"
#include "CVector.h"

#include "CModel.h"

//static string head = ""

namespace ZR
{
	CModel::CModel():_mapModelPoints(compareMethod)
	{
	}


	CModel::~CModel()
	{
		_mapModelPoints.clear();
		_listFacets.clear();
		_setModelPoints.clear();
		_vectorModelPoints.clear();
	}

	void CModel::AddPoints(std::shared_ptr<CVertex> &vertex,const std::shared_ptr< CFacet> &facet)
	{
		if (vertex != nullptr &&facet != nullptr)
		{
			auto iter = _mapModelPoints.insert(std::make_pair(vertex, 0));

			if (!iter.second)
			{//	map中已经存在当前点
				auto pos = _mapModelPoints.find(vertex);
				if (pos != _mapModelPoints.end())
				{
					pos->first->AddFaces(facet);
					facet->InsertVertex(pos->first);		//	当map容器中存在相同的点时，应当插入容器中的点，而不是新建立的vertex，（保持CFacet 和_mapModelPoints中的点的一致性）
					vertex = nullptr;						//	后面不需要使用，则立即释放内存
				}
			}
			else
			{//	新增点成功，即map中不存在当前点
				facet->InsertVertex(vertex);
				vertex->AddFaces(facet);										//为点添加所属的面
			}
		}
			

	}
	void CModel::AddPoints(std::shared_ptr<CVertex> &vertex,const size_t &num)
	{
		_mapModelPoints.insert(std::make_pair(vertex, num));
	}

	void CModel::AddFacet(const std::shared_ptr< CFacet> &facet)
	{
		_listFacets.push_back(facet);
	}

	void CModel::SerialVertex()
	{
		//	给value赋值		
		int value = 0;
		for (std::map< std::shared_ptr<CVertex>, long long, CVertexCmp>::iterator iter = _mapModelPoints.begin(); iter != _mapModelPoints.end(); iter++)
		{
			iter->first->SetNumber(value);
			iter->second = value++;
		}
	}

	size_t CModel::GetTerritory(double x, double y, double z, int n)									//	获取指定点的n阶领域
	{
		return 0;
	}

	size_t CModel::GetTerritory(const std::shared_ptr< CVertex> &vertex, const int &n)
	{
		size_t result = 0;
		std::set<std::shared_ptr<CVertex>, CVertexCmp> tempSet;
		std::set<std::shared_ptr<CVertex>, CVertexCmp> tempSetBack;

		tempSet.insert(vertex);
		tempSetBack.insert(vertex);

		for (int i = 0; i < n; ++i)
		{
			if (i % 2 == 0)
			{
				for (auto iter : tempSet)
				{
					iter->GetNearPoints(tempSetBack);
					//std::cout << "删除指定元素" << std::endl;
				}

				//	给当前点添加指向下一个节点
				
				for (auto iter : tempSetBack)
				{
					long num = iter->GetNum();
					if(iter != vertex)
						vertex->AddNextVertex(iter);
				}
				
				result += tempSetBack.size() - 1;
				tempSet.clear();

			}
			else
			{
				for (auto iter : tempSetBack)
				{
					iter->GetNearPoints(tempSet);
				}
				result += tempSet.size() - 1;
				tempSetBack.clear();
			}
		}
		return result;

	}
	size_t CModel::GetTerritory(const int &pointNum, const int  &n)
	{
		//size_t  result = 0;
		std::shared_ptr< CVertex> vertex;
		auto pos = std::find_if(_mapModelPoints.begin(), _mapModelPoints.end(), [&](const std::pair<std::shared_ptr< CVertex>, long long> &pairPoint) { return ((pairPoint.second == pointNum) ? true : false);   });//ptrVertex->GetNum() == pointNum) ? true : false);
		if (pos == _mapModelPoints.end())
			return 0;

		vertex = pos->first;
		
		return GetTerritory(vertex, n);
	}

	void CModel::ReadBuffer(fs::path &fileDir,char **buffer)
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

		/* 获取文件大小 */
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		/* 分配内存存储整个文件 */
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
	void CModel::WriteToPlyFile(fs::path  fileName, int containType)
	{

		if (!fs::exists(fileName.c_str()))
		{
			std::cout << "文件不存在" << std::endl;
		//	fs::(fileName.c_str());
		}
		ofstream fout(fileName.c_str());
		fout.precision(std::numeric_limits<double>::digits10);
		std::cout << "精度:" << std::numeric_limits<long double>::digits10 << std::endl;
		if(fout.is_open())
		{
			fout <<"ply" <<endl;
			fout << "format ascii 1.0" << endl;
			fout << "comment created by platoply" << endl;
			fout << "element vertex " << GetPoints(containType) << endl;
			fout << "property float32 x" << endl;
			fout << "property float32 y" << endl;
			fout << "property float32 z" << endl;
			fout << "element face " << GetnumberOfFacets() << endl;
			fout << "property list uint8 int32 vertex_indices\nend_header" << endl;
			
			int faceNuber = _listFacets.size();
			switch (containType)
			{
			case VECTOR:
				for (auto iter : _vectorModelPoints)
				{
					fout << iter->GetX() << " " << iter->GetY() << " " << iter->GetZ() << endl;
				}
				for (auto iter : _listFacets)
				{
					faceNuber--;
					std::cout << "faceNuber:" << faceNuber <<std::endl;
					int nums = iter->GetvertexNums();
					fout << nums << " ";
					for (int i = 0; i < nums; ++i)
					{
						int times = 0;
						//auto iter = std::find(_vectorModelPoints.begin(), _vectorModelPoints.end(), [&](const CVertex vertex) { if (vertex == iter.GetVertex(i))return true; });
						/*std::shared_ptr<CVertex> vertexPtr = iter->GetVertex(i);
						for (auto it : _vectorModelPoints)
						{
							if (it == vertexPtr)
							{
								fout << it->GetNum() << " ";
								break;
							}
						}
						*/
						auto pos = std::find(_vectorModelPoints.begin(), _vectorModelPoints.end(), iter->GetVertex(i));
						if(pos != _vectorModelPoints.end())
							fout << (*pos)->GetNum() << " ";
						
					}
					fout << endl;

				}
				break;
			case MAP:
				for (auto iter : _mapModelPoints)
				{
					fout << iter.first->GetX() << " " << iter.first->GetY() << " " << iter.first->GetZ() << endl;
				}

				for (auto iter : _listFacets)
				{
					int nums = iter->GetvertexNums();
					fout << nums << " ";
					for (int i = 0; i < nums; ++i)
					{
						auto pos = _mapModelPoints.find(iter->GetVertex(i));
						if (pos != _mapModelPoints.end())
							fout << pos->second << " ";

					}
					fout << endl;

				}
				break;
			case SET:
				for (auto iter : _setModelPoints)
				{
					fout << iter->GetX() << " " << iter->GetY() << " " << iter->GetZ() << endl;
				}

				for (auto iter : _listFacets)
				{
					int nums = iter->GetvertexNums();
					fout << nums << " ";
					for (int i = 0; i < nums; ++i)
					{
						auto pos = _setModelPoints.find(iter->GetVertex(i));
						if (pos != _setModelPoints.end())
						{
						//	int nPosition = std::distance(_setModelPoints.begin(), pos);
							//int num = (*pos)->GetNum;
							fout << (*pos)->GetNum() << " ";
						}						
					}
					fout << endl;

				}
				break;
			}
		

			fout.close();
		}
	}

	void CModel::SetOppositeEdge(const std::shared_ptr< CFacet> &facet, int &num)
	{
		char edge = 0;
		std::shared_ptr< CVertex> ptrVertex0, ptrVertex1, ptrVertex2;
		std::shared_ptr< CFacet> tempFacet = nullptr;

		ptrVertex0 = facet->GetVertex(0);
		ptrVertex1 = facet->GetVertex(1);
		ptrVertex2 = facet->GetVertex(2);
		switch (num)
		{
		case 1:	//	只有第一个点插入失败
			break;
		case 2:	//	只有第二个点插入失败
			break;
		case 4:	//	只有第三个点插入失败
			break;
		case 3:	//	第一、二两个点插入失败
			tempFacet = ptrVertex0->GetFaceNumByVertex(ptrVertex0, ptrVertex1, edge);	
			if (tempFacet)
			{
				facet->SetOppositeHEdge(std::make_pair(tempFacet, 0));
				tempFacet->SetOppositeHEdge(std::make_pair(facet, edge));
			}
			break;
		case 5:	//	第1，3两个点插入失败
			tempFacet = ptrVertex0->GetFaceNumByVertex(ptrVertex0, ptrVertex2,edge);
			if (tempFacet)
			{
				facet->SetOppositeHEdge(std::make_pair(tempFacet, 2));
				tempFacet->SetOppositeHEdge(std::make_pair(facet, edge));
			}
			break;
		case 6:	//	第2、3两个点插入失败
			tempFacet = ptrVertex1->GetFaceNumByVertex(ptrVertex1, ptrVertex2,edge);
			if (tempFacet)
			{
				facet->SetOppositeHEdge(std::make_pair(tempFacet, 1));
				tempFacet->SetOppositeHEdge(std::make_pair(facet, edge));
			}
			break;
		case 7:			///三个点都插入失败
			tempFacet = ptrVertex0->GetFaceNumByVertex(ptrVertex0, ptrVertex1, edge);
			if (tempFacet)
			{
			//	facet->SetOppositeHEdge(std::make_pair(tempFacet, 0));
				tempFacet->SetOppositeHEdge(std::make_pair(facet, edge));
			}
			tempFacet = ptrVertex0->GetFaceNumByVertex(ptrVertex0, ptrVertex2, edge);
			if (tempFacet)
			{
				//facet->SetOppositeHEdge(std::make_pair(tempFacet, 2));
				tempFacet->SetOppositeHEdge(std::make_pair(facet, edge));
			}
			tempFacet = ptrVertex1->GetFaceNumByVertex(ptrVertex1, ptrVertex2, edge);
			if (tempFacet)
			{
				//facet->SetOppositeHEdge(std::make_pair(tempFacet, 1));
				tempFacet->SetOppositeHEdge(std::make_pair(facet, edge));
			}	
			break;
		default:
			break;
		
		}


	}
	void CModel::Calculate(fs::path fileDir, int containType)
	{
		
		char *buffer = nullptr;
		ReadBuffer(fileDir, &buffer);
	//	_numberOfFacets = 0;
		long double x, y, z;
		unsigned long facetNum = 0;
		std::map< std::shared_ptr<CVertex>, long long, CVertexCmp>::iterator pos;
		char edge = 0;
		int i;
		int num = 0;	//	用来判断map插入重复点的位置（num = 1（0,1重复），num = 3 (1,2重复)，num = 2(0,2重复)）
		unsigned long tempNum = 0;
		string name, useless;
		stringstream ss(buffer);
		ss.precision(std::numeric_limits<long double>::digits10);
		// 去掉首行无用数据
		getline(ss, name);
		//ss.get();
		do {
			ss >> useless;
			if (useless != "facet")
				break;
			//_numberOfFacets++;
			getline(ss, useless);
			getline(ss, useless);

			std::shared_ptr<CFacet> oneFace(new CFacet);
			oneFace->SetNumber(facetNum++);
			num = 0;		// 重新赋值为零
			for (i = 0; i <3; i++)
			{
				ss >> useless >> x >> y >> z;
				//_vectorPoints.push_back(std::make_tuple(x, y, z));
				std::shared_ptr<CVertex> vertex(new CVertex(x, y, z, 0));
				vertex->AddFaces(oneFace);										//为点添加所属的面
			
				//	插入点
				
				switch (containType)
				{
				case VECTOR:
					_vectorModelPoints.push_back(vertex);
					break;
				case MAP:
				{
					auto iter = _mapModelPoints.insert(std::make_pair(vertex, 0));
					if(!iter.second)
					{
						num = num | (1 << i);
						pos = _mapModelPoints.find(vertex);
						if (pos != _mapModelPoints.end())
						{
							pos->first->AddFaces(oneFace);
							oneFace->InsertVertex(pos->first);							//	当map容器中存在相同的点时，应当插入容器中的点，而不是新建立的vertex，（保持CFacet 和_mapModelPoints中的点的一致性）
							vertex = nullptr;						//	后面不需要使用，则立即释放内存
						}
					}
					else
					{
						oneFace->InsertVertex(vertex);
					}
					break;
				}				
				case SET:
					_setModelPoints.insert(vertex);
					break;
				default:
					return;
				}
				
			}
			_listFacets.push_back(oneFace);			
			// 为没个顶点添加指向
			//oneFace->GetVertex(0)->AddNextVertex(oneFace->GetVertex(1));
			//oneFace->GetVertex(1)->AddNextVertex(oneFace->GetVertex(2));
			//oneFace->GetVertex(2)->AddNextVertex(oneFace->GetVertex(0));
			//SetOppositeEdge(oneFace, num);
			getline(ss, useless);
			getline(ss, useless);
			getline(ss, useless);
		} while (1);

		free(buffer);
		
		//	给value赋值
		if (containType == MAP)
		{
			int value = 0;
			for (std::map< std::shared_ptr<CVertex>, long long, CVertexCmp>::iterator iter = _mapModelPoints.begin(); iter != _mapModelPoints.end(); iter++)
			{
				iter->first->SetNumber(value);
				iter->second = value++;
			}
		}
		else if (containType == VECTOR)
		{
			CVertexCmp cmp;
			sort(_vectorModelPoints.begin(), _vectorModelPoints.end(), cmp);
			_vectorModelPoints.erase(unique(_vectorModelPoints.begin(), _vectorModelPoints.end(), [](std::shared_ptr<CVertex> ptr1, std::shared_ptr<CVertex> ptr2) {return (ptr1->GetX() == ptr2->GetX() && ptr1->GetY() == ptr2->GetY() && ptr1->GetZ() == ptr2->GetZ()); }), _vectorModelPoints.end());
			int times = 0;
			for (auto iter : _vectorModelPoints)
			{
				iter->SetNumber(times);
				times++;
			}
		}
		else if (containType == SET)
		{
			size_t times = 0;
			for (auto iter : _setModelPoints)
			{
				iter->SetNumber(times);
				times++;
			}
		}

		


	}

	void CModel::Calculate2(fs::path fileDir, int containType)
	{

		char *buffer = nullptr;
		ReadBuffer(fileDir, &buffer);
		//	_numberOfFacets = 0;
		long double x, y, z;
		unsigned long facetNum = 0;
		std::map< std::shared_ptr<CVertex>, long long, CVertexCmp>::iterator pos;
		int i;
		int num = 0;	//	用来判断map插入重复点的位置（num = 3（0,1重复），num = 5(1,2重复)，num = 2(0,2重复)）
		unsigned long tempNum = 0;
		std::shared_ptr< CVertex> ptrVertex0, ptrVertex1, ptrVertex2;
		string name, useless;
		stringstream ss(buffer);
		ss.precision(std::numeric_limits<long double>::digits10);
		// 去掉首行无用数据
		getline(ss, name);
		//ss.get();
		do {
			ss >> useless;
			if (useless != "facet")
				break;
			//_numberOfFacets++;
			getline(ss, useless);
			getline(ss, useless);

			std::shared_ptr<CFacet> oneFace(new CFacet);
			std::shared_ptr<CVertex> vertexX(new CVertex()), vertexY(new CVertex()), vertexZ(new CVertex());

			oneFace->SetNumber(facetNum++);
			num = 0;		// 重新赋值为零
			for (i = 1; i <= 3; i++)
			{
				ss >> useless >> x >> y >> z;
				if (i == 1)
				{
					vertexX->SetVertex(x, y, z,0);
					vertexX->AddFaces(oneFace);				//为点添加所属的面
					oneFace->InsertVertex(vertexX);
					auto iter = _mapModelPoints.insert(std::make_pair(vertexX, 0));
					if (!iter.second)
					{
						num += i;
						pos = _mapModelPoints.find(vertexX);
						if (pos != _mapModelPoints.end())
						{
							pos->first->AddFaces(oneFace);
						//	pos->first->AddNextVertex(vertexY);

						}
					}

				}
				else if (i == 2)
				{
					vertexY->SetVertex(x, y, z, 0);
					vertexY->AddFaces(oneFace);				//为点添加所属的面
					oneFace->InsertVertex(vertexY);
					auto iter = _mapModelPoints.insert(std::make_pair(vertexY, 0));
					if (!iter.second)
					{
						num += i;
						pos = _mapModelPoints.find(vertexY);
						if (pos != _mapModelPoints.end())
						{
							pos->first->AddFaces(oneFace);
							//pos->first->AddNextVertex(vertexZ);
						}
					}
					
				}
				else if (i == 3)
				{
					vertexZ->SetVertex(x, y, z, 0);
					vertexZ->AddFaces(oneFace);				//为点添加所属的面
					oneFace->InsertVertex(vertexZ);
					auto iter = _mapModelPoints.insert(std::make_pair(vertexZ, 0));
					if (!iter.second)
					{
						num += i;
						pos = _mapModelPoints.find(vertexZ);
						if (pos != _mapModelPoints.end())
						{
							pos->first->AddFaces(oneFace);
						//	pos->first->AddNextVertex(vertexX);
						}
					}
				}
			
		
			}
			_listFacets.push_back(oneFace);
		


			getline(ss, useless);
			getline(ss, useless);
			getline(ss, useless);
		} while (1);

		free(buffer);

		//	给value赋值
		if (containType == MAP)
		{
			int value = 0;
			for (std::map< std::shared_ptr<CVertex>, long long, CVertexCmp>::iterator iter = _mapModelPoints.begin(); iter != _mapModelPoints.end(); iter++)
			{
				iter->first->SetNumber(value);
				iter->second = value++;
			}
		}
		else if (containType == VECTOR)
		{
			CVertexCmp cmp;
			sort(_vectorModelPoints.begin(), _vectorModelPoints.end(), cmp);
			_vectorModelPoints.erase(unique(_vectorModelPoints.begin(), _vectorModelPoints.end(), [](std::shared_ptr<CVertex> ptr1, std::shared_ptr<CVertex> ptr2) {return (ptr1->GetX() == ptr2->GetX() && ptr1->GetY() == ptr2->GetY() && ptr1->GetZ() == ptr2->GetZ()); }), _vectorModelPoints.end());
			int times = 0;
			for (auto iter : _vectorModelPoints)
			{
				iter->SetNumber(times);
				times++;
			}
		}
		else if (containType == SET)
		{
			size_t times = 0;
			for (auto iter : _setModelPoints)
			{
				iter->SetNumber(times);
				times++;
			}
		}




	}


	/*
	// 文件获取临界不截断的真正大小
	static streamsize inline getRealSize(ifstream* file, streamoff start, streamsize size)
	{
			file->seekg(start + size);
			while (words[file->get()])
	        ++size;
		    return size;
	}
	static streamsize inline getBlockSize(int step, streamoff start, streamsize size)
	{
		char* p = loadedFile[step] + start + size;
	   while (words[*p]) {
			++size;
			++p;
		}
		return size;
	}
	static void readBlock(int step, int id, streamoff start, streamsize size)
	{

	}
	void CModel::ReadAscallStlFile(CReadStlFile &reader)
	{
		
		ios::sync_with_stdio(false);
		int threadCount = 4;						//	并行计算的线程数量
		ifstream file;
		streamoff start = 0;
		streamsize realsize = 0;
		streamoff index, part,len;
		streamsize maxsize = BLOCKSIZE + 256;
		bool needWait = false,switchBlock = false;					// 用于控制缓存block1 he block2的切换
		bool words[128];
		size_t fileSize = reader.GetFileSize();


		// 初始化可识别字符
		memset(words, false, 128);
		for (char c = 97; c != 123; ++c)
		    words[c] = true;
		for (char c = 65; c != 91; ++c)
			words[c] = true;
		for (char c = 48; c != 58; ++c)
			 words[c] = true;
		words['-'] = true;
		words['.'] = true;
		//	双缓存
		std::shared_ptr<char> block1(new char[BLOCKSIZE + 256], [](char *p) {delete[] p; });
		std::shared_ptr<char> block2(new char[BLOCKSIZE + 256], [](char *p) {delete[] p; });
		std::shared_ptr<thread>  threads(new thread[threadCount], [](std::thread *p) { delete[] p; });

		while (fileSize)
		{
			file.seekg(start);
			realsize = fileSize > maxsize ? getRealSize(&file, start, BLOCKSIZE) : fileSize;

			if (switchBlock) {
				file.read(&*block1, realsize);
			}
			else {
				file.read(&*block2, realsize);
			}
			start += realsize;
			fileSize -= realsize;

			// 等待
			if (needWait)
				for (int i = 0; i < threadCount; ++i) threads.get()[i].join();
			else
				needWait = true;
			//计算
			index = 0, part = realsize / threadCount;
			for (int i = 1; i < threadCount; ++i)
			{
				 len = getBlockSize(switchBlock, index, part);
				threads.get()[i] = std::thread(readBlock, switchBlock, i, index, len);
				index += len;
			}
			threads.get()[0] = thread(readBlock, switchBlock, 0, index, realsize - index);
			switchBlock = !switchBlock;

		}
		for(int i = 0; i < threadCount; ++i) threads.get()[i].join();
		file.close();

	}

	void CModel::ReadBinaryFile(CReadStlFile &reader)
	{

	}
	
	void CModel::WriteToPlyFile(CReadStlFile &reader)
	{

	}

	void CModel::ReadFile()
	{
		clock_t start = clock();
		ifstream fin(objpath, std::ios::binary);

		vector<char> buf(fin.seekg(0, std::ios::end).tellg());
		fin.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));

		fin.close();
		clock_t end = clock();
		cout << "time : " << ((double)end - start) / CLOCKS_PER_SEC << "s\n";
	}

	*/

	size_t CModel::GetPoints(int containType)
	{
		if (containType == MAP)
			return _mapModelPoints.size();
		else if (containType == VECTOR)
			return _vectorModelPoints.size();
		else if (containType == SET)
			return _setModelPoints.size();
		
	}
	size_t CModel::GetNumOfVertes()
	{
		return _mapModelPoints.size();
	}
	size_t CModel::GetnumberOfFacets()
	{
		return _listFacets.size();
	}
	
	void CModel::test()
	{

		//	寻找孔洞
		std::vector< std::list<std::shared_ptr< CVertex>>> result;
		GetHoles2(result);
		for (int i = 0; i < result.size(); i++)
		{
			std::cout << "孔洞数" << i << "组成的点的个数："<<result[i].size()<<std::endl;
		}
		result.clear();
	}

	int CModel::GetHoles()
	{
		int result = 0;
		unsigned long faceNum = 0;
		std::list< std::shared_ptr<CFacet>> borderFaces;
		std::shared_ptr< CVertex> vStart = nullptr,vEnd = nullptr;

		//	找到所有的
		std::copy_if(_listFacets.begin(), _listFacets.end(), back_inserter(borderFaces), [](const std::shared_ptr<CFacet> &facet) { return (facet->IsBorder()) ? true : false; });

		std::cout << "borderFaces.size:" << borderFaces.size()<< std::endl;
		(*borderFaces.begin())->GetBorderVertex(vStart, vEnd);
		std::shared_ptr< CVertex> tempStart = vStart, tempEnd = vEnd;
		borderFaces.erase(borderFaces.begin());

	
		
		//for (std::list< std::shared_ptr<CFacet>>::iterator iter = borderFaces.begin(); iter!= borderFaces.end();++iter)
		while(borderFaces.size())
		{			
			while (tempStart != nullptr && tempEnd != nullptr &&tempStart != tempEnd)
			{
				auto pos = std::find_if(borderFaces.begin(), borderFaces.end(), [&](const std::shared_ptr<CFacet> &facet) { return (facet->IsExist(vEnd)); });
				if (pos != borderFaces.end())
				{
					(*pos)->GetBorderVertex(vStart, vEnd);
					borderFaces.erase(pos);
					tempEnd = (vStart == tempEnd) ? vEnd : vStart;
				}
				
			}
			result++;

		}
		return result;
	}
	void CModel::GetHoles2(std::vector<std::list<std::shared_ptr< CVertex>>> & result)
	{
		std::list < std::shared_ptr< CVertex>> listTemp;			//	临时存储查找点的领域节点
		std::list < std::shared_ptr< CVertex>> listTempback;
		std::list<std::shared_ptr< CVertex>> listVertexs;			//	所有的边界节点

		// 查找并存储所有孔洞的边界点
		for (auto iter : _mapModelPoints)
		{
			if(iter.first->GetFacetNum() != (GetTerritory(iter.first,1)))
			{
				listVertexs.push_back(iter.first);
			}			
		}
		std::shared_ptr< CVertex> vStart, vEnd = nullptr,temp = nullptr;
		
		std::cout << "边界点个数：" << listVertexs.size() << std::endl;

		//	对孔洞进行循环遍历，形成环则判断为一个孔洞
		while (listVertexs.size())
		{
			std::list<std::shared_ptr< CVertex>> hole;			//	孔洞
			(*listVertexs.begin())->GetNextVertex(listTemp); 
			(*listVertexs.begin())->GetNextVertex(listTempback);
			hole.push_front(*listVertexs.begin());
			listVertexs.erase(listVertexs.begin());
			//std::cout << "start:" << vStart->GetNum() << std::endl;

			do {
				bool flag1 = false, flag2 = false;
				for (auto iter : listTempback)
				{
					auto pos = std::find(listVertexs.begin(), listVertexs.end(), iter);
					if (pos != listVertexs.end())
					{
						flag1 = true;
						vStart = iter; 
						hole.push_front(vStart);
						vStart->GetNextVertex(listTempback);
						listVertexs.erase(pos);
						break;
					}
				}
				for (std::list < std::shared_ptr< CVertex>>::reverse_iterator iter = listTemp.rbegin(); iter != listTemp.rend(); ++iter)
				{
					auto pos = std::find(listVertexs.begin(), listVertexs.end(), *iter);
					if (pos != listVertexs.end())
					{
						flag2 = true;
						vEnd = *pos;
						hole.push_front(vEnd);

						vEnd->GetNextVertex(listTemp);
						listVertexs.erase(pos);
						break;
					}
				}
				if (flag1 == false && flag2 == false)
					break;

			} while (vStart != vEnd );
			//std::cout << "组成孔洞的点的个数：" << hole.size() << std::endl;
			result.push_back(hole);
			hole.clear();

		}
		listVertexs.clear();

	}

	void CModel::GetData(ofstream &fout)
	{
		for (auto iter : _mapModelPoints)
		{
			fout << iter.first->GetX() << " " << iter.first->GetY() << " " << iter.first->GetZ() << endl;
		}

		for (auto iter : _listFacets)
		{
			int nums = iter->GetvertexNums();
			fout << nums << " ";
			for (int i = 0; i < nums; ++i)
			{
				auto pos = _mapModelPoints.find(iter->GetVertex(i));
				if (pos != _mapModelPoints.end())
					fout << pos->second << " ";

			}
			fout << endl;

		}
	}
	float CModel::CalculateVolume()
	{
		float sumVolum = 0.0f;
		float v321 = 0.0f, v231 = 0.0f, v312 = 0.0f, v132 = 0.0f, v213 = 0.0f, v123 = 0.0f;
		CVector v0, v1, v2;
		for (auto iter : _listFacets)
		{
			v0.SetVector(iter->GetVertex(0)->GetX() , iter->GetVertex(0)->GetY(),iter->GetVertex(0)->GetZ());
			v1.SetVector(iter->GetVertex(1)->GetX(), iter->GetVertex(1)->GetY() ,  iter->GetVertex(1)->GetZ());
			v2.SetVector(iter->GetVertex(2)->GetX(), iter->GetVertex(2)->GetY(),  iter->GetVertex(2)->GetZ());
			//	计算体积
			v321 = v2.GetX() * v1.GetY() * v0.GetZ();
			v231 = v1.GetX() * v2.GetY() * v0.GetZ();
			v312 = v2.GetX() * v0.GetY() * v1.GetZ();
			v132 = v0.GetX() * v2.GetY() * v1.GetZ();
			v213 = v1.GetX() * v0.GetY() * v2.GetZ();
			v123 = v0.GetX() * v1.GetY() * v2.GetZ();
			sumVolum += (1.0f / 6.0f) * (-v321 + v231 + v312 - v132 - v213 + v123);
		}
		return sumVolum;
	}
	float CModel::CalculateSumArea()
	{
		/*
		    float v321 = pt2.x * pt1.y * pt0.z;
        float v231 = pt1.x * pt2.y * pt0.z;
        float v312 = pt2.x * pt0.y * pt1.z;
        float v132 = pt0.x * pt2.y * pt1.z;
        float v213 = pt1.x * pt0.y * pt2.z;
        float v123 = pt0.x * pt1.y * pt2.z;

        return (1.0f / 6.0f) * (-v321 + v231 + v312 - v132 - v213 + v123);*/
		float sum = 0.0f;
		CVector v1, v2, v3;
		float a, b, c, p;
		for (auto iter : _listFacets)
		{
			v1.SetVector(iter->GetVertex(1)->GetX() - iter->GetVertex(0)->GetX(), iter->GetVertex(1)->GetY() - iter->GetVertex(0)->GetY(), iter->GetVertex(1)->GetZ() - iter->GetVertex(0)->GetZ());
			v2.SetVector(iter->GetVertex(2)->GetX() - iter->GetVertex(1)->GetX(), iter->GetVertex(2)->GetY() - iter->GetVertex(1)->GetY(), iter->GetVertex(2)->GetZ() - iter->GetVertex(1)->GetZ());
			v3.SetVector(iter->GetVertex(0)->GetX() - iter->GetVertex(2)->GetX(), iter->GetVertex(0)->GetY() - iter->GetVertex(2)->GetY(), iter->GetVertex(0)->GetZ() - iter->GetVertex(2)->GetZ());
			a = sqrtf(v1.GetX()*v1.GetX() + v1.GetY() *v1.GetY() + v1.GetZ() *v1.GetZ());
			b = sqrtf(v2.GetX()*v2.GetX() + v2.GetY() *v2.GetY() + v2.GetZ() *v2.GetZ());
			c = sqrtf(v3.GetX()*v3.GetX() + v3.GetY() *v3.GetY() + v3.GetZ() *v3.GetZ());
			p = (a + b + c) * 0.5f;
			sum += sqrt(p * (p - a) * (p - b) * (p - c));

		
		}

		return sum;
	}
}
