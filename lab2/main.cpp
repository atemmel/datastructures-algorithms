#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>

template<typename ForwardIt>
void selectionSort(ForwardIt first, ForwardIt last)
{
	for(; first != last; first++)
	{
		auto min = std::min_element(first, last);
		std::swap(*first, *min);
	}
}

template<typename RandomAccessIt>
void insertionSort(RandomAccessIt first,RandomAccessIt last)
{
	for(auto it = first + 1; it != last; it++)
	{
		auto value = *it;
		auto insert = it - 1;
		
		//För varje element till insättningspunkten (baklänges)
		for(; insert >= first && *insert > value; insert--) 
		{
			//Flytta varje element åt höger
			*(insert + 1) = *insert;
		}

		*(insert + 1) = value;
	}
}

template<typename ForwardIt>
void printRange(ForwardIt first, ForwardIt last)
{
	std::cout << '{';
	for(; first != last; first++)
		std::cout << *first << ", ";
	std::cout << "}\n";
}

int main()
{
	std::vector<int> vector = {0, 7, 8, 1, 3, 6, 2, 5, 3, 4, 0};

	printRange(vector.begin(), vector.end() );

	insertionSort(vector.begin(), vector.end() );

	printRange(vector.begin(), vector.end() );

	return 0;
}
