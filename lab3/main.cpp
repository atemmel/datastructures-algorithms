#include "stopwatch.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

template<int N>
struct Sieve
{
	Sieve()
		: m_bits(N, true), m_it(m_bits.begin() )
	{
		for(int i = 2; i < limit; i++)
		{
			if(m_bits[i])
			{
				for(int j = i * i; j < N; j += i)
				{
					m_bits[j] = false;
				}
			}
		}
	}

	int operator()()
	{
		auto next = std::find(std::next(m_it), m_bits.end(), true);

		if(next == m_bits.end() ) return std::distance(m_bits.begin(), m_it);

		m_it = next;

		return std::distance(m_bits.begin(), m_it);
	}

private:
	std::vector<bool> m_bits;
	std::vector<bool>::iterator m_it;

	static constexpr auto limit = std::sqrt(N);
};

int main()
{
	Sieve<100> sieve;

	for(int i = 0; i < 9; i++)
	{
		std::cout << sieve() << '\n';
	}

	return 0;
}
