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

template<typename ForwardIt, typename T>
ForwardIt linearSearch(ForwardIt first, ForwardIt last, T value)
{
	for(auto it = first; it != last; it++)
	{
		if(*it == value) return it;
	}
	return last;
}

template<typename ForwardIt, typename T>
ForwardIt binarySearch(ForwardIt first, ForwardIt last, T value)
{
	auto left = first, right = std::prev(last);

	while(left <= right)
	{
		auto middle = left + std::distance(left, right) / 2;

		if(value > *middle) left = std::next(middle);
		else if(value < *middle) right = std::prev(middle);
		else return middle;
	}

	return last;
}

int main()
{
	std::vector<int> vector(1000);
	std::generate(vector.begin(), vector.end(), Sieve<1 << 15>() );

	for(auto i : vector) std::cout << i << ' ';
	std::cout << '\n' << std::boolalpha;

	std::cout << "waawa\n";
	std::cout << ( linearSearch(vector.begin(), vector.end(), 11) != vector.end() ) << '\n';
	std::cout << "waawa\n";
	std::cout << ( binarySearch(vector.begin(), vector.end(), 11) != vector.end() ) << '\n';

	return 0;
}
