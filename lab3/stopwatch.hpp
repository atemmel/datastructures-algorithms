#pragma once
#include <chrono>
#include <sstream>

template<typename Time>
class Stopwatch
{
public:
	double get() const
	{
		auto end = Clock::now();
		return std::chrono::duration<double>(end - m_start).count();
	}

	double reset()
	{
		double time = get();
		m_start = Clock::now();
		return time;
	}

private:
	using Clock = std::chrono::high_resolution_clock;

	std::chrono::time_point<Clock> m_start = Clock::now();
};
