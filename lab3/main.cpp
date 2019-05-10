#include "stopwatch.hpp"
#include "bst.hpp"
#include "hash.hpp"

#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <limits>
#include <thread>
#include <cmath>

using namespace std::chrono_literals;
	
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

template<typename ForwardIterator>
struct LinearSearch
{
	LinearSearch(ForwardIterator first, ForwardIterator last)
	: m_begin(first), m_end(last)
	{}

	template<typename T>
	ForwardIterator find(T value)
	{
		for(auto it = m_begin; it != m_end; it++)
		{
			if(*it == value) return it;
		}
		return m_end;
	}

	ForwardIterator m_begin, m_end;
};

template<typename ForwardIterator>
struct BinarySearch
{
	BinarySearch(ForwardIterator first, ForwardIterator last)
	: m_begin(first), m_end(last)
	{}

	template<typename T>
	ForwardIterator find(T value)
	{
		auto left = m_begin, right = std::prev(m_end);

		while(left <= right)
		{
			auto middle = left + std::distance(left, right) / 2;

			if(value > *middle) left = std::next(middle);
			else if(value < *middle) right = std::prev(middle);
			else return middle;
		}

		return m_end;
	}

	ForwardIterator m_begin, m_end;
};

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

void wait(bool &completed, std::chrono::duration<double> time)
{
	std::this_thread::sleep_for(time);
	completed = true;
}

template<typename Search, typename T>
std::uintmax_t bench(Search &search, const std::vector<T> & primes, const size_t limit, std::chrono::duration<double> time)
{
	bool completed = false;
	std::mt19937 gen(std::random_device{}() );
	std::uniform_int_distribution<int> dist(0, limit );
	std::uintmax_t iterations = 0;
	T dummy = T();

	std::thread timer(std::bind(wait, std::ref(completed), time) );
	do
	{
		T value = primes[dist(gen)];
		auto it = search.find(value);
		dummy += *it, ++iterations;
	}
	while(!completed);

	timer.join();

	std::cout << "Ran " << iterations << " times in " << time.count() << " second\n";
	std::cout << "Dummy output: " << dummy << "\n\n";

	return iterations;
}

int main()
{
	std::ifstream file("primes");
	std::vector<int> vector;
	using iter = decltype(vector.begin() );

	{
		std::cout << "Reading primes from file...\n";
		int value;
		Stopwatch<std::chrono::seconds> watch;
		while(file >> value) vector.push_back(value);
		auto t = watch.reset();
		std::cout << vector.size() << " primes read, took " << t << " seconds\n\n";
	}

	{
		std::cout << "Linear search\n";
		LinearSearch<iter> linear(vector.begin(), vector.end() );

		bench(linear, vector, vector.size(), 1s);
	}

	{
		std::cout << "Binary search\n";
		BinarySearch<iter> binary(vector.begin(), vector.end() );

		bench(binary, vector, vector.size(), 1s);
	}

	{
		std::cout << "Creating BST\n";
		BinarySearchTree<int> tree(vector.begin(), vector.end() );

		bench(tree, vector, vector.size(), 1s);
	}

	{
		std::cout << "Creating Hashtable\n";
		HashTable<int> hash(vector.begin(), vector.end() );

		bench(hash, vector, vector.size(), 1s);
	}

	/*
	//std::vector<int> vector(100);
	//std::generate(vector.begin(), vector.end(), Sieve<1 << 10>() );
	BinarySearchTree<int> tree(vector.begin(), vector.end() );

	Stopwatch watch;
	std::cout << *tree.find(2) << '\n';
	std::cout << "BST took: " << watch.reset() << "µs\n";
	watch.reset();
	std::cout << *linearSearch(vector.begin(), vector.end(), 123891263) << '\n';
	std::cout << "Linear search took: " << watch.reset() << "µs\n";

	*/

	/*
	Stopwatch watch;
	std::cout << *hash.find(2) << '\n';
	double time = watch.reset();
	std::cout << "Hash took: " << time << "µs\n";
	*/


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
