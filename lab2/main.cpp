#include "generators.hpp"
#include "stopwatch.hpp"
#include "sorts.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

template<typename Sort, typename Populate>
void benchmark(Sort sort, Populate populate, unsigned iterations, unsigned size)
{
	//Header
	for(unsigned i = 1; 1 <= iterations; i++) std::cout << size * j << ',';
	std::cout << '\n';

	for(unsigned j = 1; j <= iterations; j++)
	{
		std::vector<int> vector(size * j);

		for(unsigned i = 0; i < iterations; i++)
		{
			std::generate(vector.begin(), vector.end(), populate);

			Stopwatch watch;
			sort(vector.begin(), vector.end() );

			std::cout << watch.getSeconds() << ',';
		}
		std::cout << "\n";
	}
}

int main()
{
	auto insertionLambda = [](auto first, auto last)
	{
		insertionSort(first, last);
	};
	
	auto selectionLambda = [](auto first, auto last)
	{
		selectionSort(first, last);
	};

	auto partitionLambda = [](auto first, auto last)
	{
		partitionSort(first, last);
	};

	auto medianPartitionLambda = [](auto first, auto last)
	{
		medianPartitionSort(first, last);
	};

	auto stdsortLambda = [](auto first, auto last)
	{
		std::sort(first, last);
	};

	unsigned seed = std::random_device{}();
	unsigned iterations = 5;
	unsigned size = 1 << 15;

	std::ofstream file;
	std::ostringstream stream;
	auto oldBuff = std::cout.rdbuf();

	std::cout.rdbuf(stream.rdbuf() );

	/*
	std::cout << "|| Random ||\n\n";
	std::cout << "Insertion:\t";
	benchmark(insertionLambda, Random(seed), iterations, size);
	std::cout << "Selection:\t";
	benchmark(selectionLambda, Random(seed), iterations, size);
	std::cout << "Partition (Right):\t";
	benchmark(partitionLambda, Random(seed), iterations, size);
	std::cout << "Partition (Median):\t";
	benchmark(medianPartitionLambda, Random(seed), iterations, size);
	std::cout << "std::sort:\t";
	benchmark(stdsortLambda, Random(seed), iterations, size);

	std::cout << "|| Monotonic Growing ||\n\n";
	std::cout << "Insertion:\t";
	benchmark(insertionLambda, MonoGrowing(seed), iterations, size);
	std::cout << "Selection:\t";
	benchmark(selectionLambda, MonoGrowing(seed), iterations, size);
	std::cout << "Partition (Right):\t";
	benchmark(partitionLambda, MonoGrowing(seed), iterations, size);
	std::cout << "Partition (Median):\t";
	benchmark(medianPartitionLambda, MonoGrowing(seed), iterations, size);
	std::cout << "std::sort:\t";
	benchmark(stdsortLambda, MonoGrowing(seed), iterations, size);

	std::cout << "|| Monotonic Falling ||\n\n";
	std::cout << "Insertion:\t";
	benchmark(insertionLambda, MonoFalling(seed), iterations, size);
	std::cout << "Selection:\t";
	benchmark(selectionLambda, MonoFalling(seed), iterations, size);
	std::cout << "Partition (Right):\t";
	benchmark(partitionLambda, MonoFalling(seed), iterations, size);
	std::cout << "Partition (Median):\t";
	benchmark(medianPartitionLambda, MonoFalling(seed), iterations, size);
	std::cout << "std::sort:\t";
	benchmark(stdsortLambda, MonoFalling(seed), iterations, size);

	std::cout << "|| Constant ||\n\n";
	std::cout << "Insertion:\t";
	benchmark(insertionLambda, Constant(seed), iterations, size);
	std::cout << "Selection:\t";
	benchmark(selectionLambda, Constant(seed), iterations, size);
	std::cout << "Partition (Right):\t";
	benchmark(partitionLambda, Constant(seed), iterations, size);
	std::cout << "Partition (Median):\t";
	benchmark(medianPartitionLambda, Constant(seed), iterations, size);
	std::cout << "std::sort:\t";
	benchmark(stdsortLambda, Constant(seed), iterations, size);
	*/
	auto log = [&](const char* path)
	{
		file.open(path);
		file << stream.str();
		stream.clear();
		file.close();
	};

	/* Insertion, random */
	benchmark(insertionLambda, Random(seed), iterations, size);
	log("insertion_random.csv");

	/* Selection, random */
	benchmark(selectionLambda, Random(seed), iterations, size);
	log("selection_random.csv");

	/* Partition (Right), random */
	benchmark(partitionLambda, Random(seed), iterations, size);
	log("partition_random.csv");

	/* Partition (Median), random */
	benchmark(medianPartitionLambda, Random(seed), iterations, size);
	log("medianPartition_random.csv");

	/* std::sort, random */
	benchmark(stdsortLambda, Random(seed), iterations, size);
	log("stdsort_random.csv");


	std::cout.rdbuf(oldBuff);

	return 0;
}
