
#include "stdafx.h"

#include "Common.h""
namespace ZR
{

	Common::Common()
	{

	}
	Common::~Common()
	{

	}

	int Common::CalculateTimeDiff(const std::chrono::system_clock::time_point &startTime, const std::chrono::system_clock::time_point endTime)
	{
		auto usingTime = endTime - startTime;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(usingTime);
		return ms.count();
	}
	/*
	int Common::CalculateTimeDiff(const time_t &startTime, const time_t &endTime)
	{
		return (double)(startTime - endTime) / CLOCKS_PER_SEC;
	}
	*/

}