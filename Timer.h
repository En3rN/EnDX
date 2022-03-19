#pragma once
#include "logger.h"
#include <chrono>
#include <string>
namespace En3rN
{	
	using ms = std::milli;
	using ns = std::nano;
	using sek = std::ratio<1>;
	using min = std::ratio<60, 1>;
	
	template  < typename retVal = float,
				typename dur = std::chrono::milliseconds >
			 				
	class Timer
	{
		
	public:		

		Timer() = default;
		Timer(std::string func) : func(func) {};
		~Timer() {
			if (func != "")
			{
				std::string out = "[Timer][" + func + "] -- " + GetElapsedString() + " --\n";
				OutputDebugString(out.c_str());
			}
		}
		void ReStart()
		{
			m_start = std::chrono::steady_clock::now();
			m_last = m_start;
		}
		
		retVal Stop()
		{
			return GetElapsed();
		}
		
		retVal GetElapsed()
		{
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration<retVal, dur> ( now - m_start);
			return elapsed.count();
		}
		
		retVal GetFrameTimer()
		{
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration<retVal, dur> (now - m_last);
			m_last = std::move(now);			
			return elapsed.count();
		}
		std::string GetFrameTimerString()
		{
			return std::string(std::to_string(GetFrameTimer()));
		}
		std::string GetElapsedString()
		{
			return std::string(std::to_string(GetElapsed()));
		}
	private:
		std::string func;
		std::chrono::steady_clock::time_point m_start = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point m_last = m_start;
	};
}

