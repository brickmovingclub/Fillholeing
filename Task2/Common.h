#pragma once

namespace ZR
{

	class Common
	{
		Common();
		~Common();
	public:
		//	计算两个时间差（毫秒为单位）
		static int CalculateTimeDiff(const std::chrono::system_clock::time_point &startTime, const std::chrono::system_clock::time_point endTime); 

		//static int CalculateTimeDiff(const time_t &startTime, const time_t &endTime);
	};
}