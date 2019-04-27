#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>

template<typename ForwardIt>
void printRange(ForwardIt first, ForwardIt last)
{
	std::cout << '{';
	for(; first != last; first++)
		std::cout << *first << ", ";
	std::cout << "}\n";
}

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

template<typename BidirectionalIt>
BidirectionalIt partition(BidirectionalIt first, BidirectionalIt last)
{
	auto pivot = *std::prev(last);
	auto ret = first;

	for(auto it = first; it < std::prev(last); it++)
	{
		if(*it < pivot)
		{
			std::iter_swap(ret, it);
			++ret;
		}
	}

	std::iter_swap(ret, std::prev(last) );
	return ret;
}

template<typename RandomAccessIt>
void partitionSort(RandomAccessIt first, RandomAccessIt last)
{
	if(first >= last) return; //Inget att sortera

	auto middle = partition(first, last);

	partitionSort(first, middle - 1);
	partitionSort(middle + 1, last);
}

int main()
{
	std::vector<int> vector = {0, 7, 8, 1, 3, 6, 2, 5, 3, 4, 0};

	printRange(vector.begin(), vector.end() );

	//insertionSort(vector.begin(), vector.end() );

	//printRange(vector.begin(), vector.end() );

	partitionSort(vector.begin(), vector.end() );

	printRange(vector.begin(), vector.end() );

	return 0;
}
