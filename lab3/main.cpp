#include "stopwatch.hpp"
#include "bst.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>

template<unsigned N>
struct Sieve
{
	Sieve()
		: m_bits(N, true), m_it(m_bits.begin() )
	{
		for(unsigned i = 2; i < limit; i++)
		{
			if(i % 10 == 0) std::cerr << "Iteration no: " << i << '\n';
			if(m_bits[i])
			{
				for(unsigned j = i * i; j < N; j += i)
				{
					if(j % 100000000 == 0) std::cerr << "\tInner iteration no: " << j << '\n';
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

void writePrimes()
{
	Sieve<std::numeric_limits<int>::max()> primes;
	int prev = 0, curr = primes();

	while(curr != prev)
	{
		std::cout << curr << ' ';
		prev = curr, curr = primes();
	}
}

int main()
{
	std::ifstream file("primes");
	std::vector<int> vector;
	int value;

	while(file >> value) vector.push_back(value);

	BinarySearchTree<int> tree(vector.begin(), vector.end() );

	Stopwatch watch;
	std::cout << *tree.find(2) << '\n';
	std::cout << "BST took: " << watch.reset() << "µs\n";
	watch.reset();
	std::cout << *linearSearch(vector.begin(), vector.end(), 123891263) << '\n';
	std::cout << "Linear search took: " << watch.reset() << "µs\n";


	/*
	std::vector<int> vector(1000);
	std::generate(vector.begin(), vector.end(), Sieve<1 << 15>() );

	for(auto i : vector) std::cout << i << ' ';
	std::cout << '\n' << std::boolalpha;

	std::cout << ( linearSearch(vector.begin(), vector.end(), 11) != vector.end() ) << '\n';
	std::cout << ( binarySearch(vector.begin(), vector.end(), 11) != vector.end() ) << '\n';

	BinarySearchTree<int> tree(vector.begin(), vector.end() );

	std::cout << tree.depth() << '\n';
	std::cout << *tree.find(2) << '\n';
	*/

	return 0;
}
