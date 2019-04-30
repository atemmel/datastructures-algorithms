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
	for(unsigned i = 1; i <= iterations; i++) std::cout << size * i << ',';
	std::cout << '\n';

	for(unsigned j = iterations; j <= iterations; j++)
	{
		std::vector<int> vector(size * j);
		printf("Iteration no: %d\n", j);

		for(unsigned i = 0; i < iterations; i++)
		{
			printf("\t Step no: %d\n", i);
			std::generate(vector.begin(), vector.end(), populate);

			Stopwatch watch;
			sort(vector.begin(), vector.end() );

			std::cout << watch.getSeconds() << ',';
		}
		std::cout << "\n";
	}

	printf("\n");
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

	auto log = [&](const char* path)
	{
		file.open(path);
		file << stream.str();
		stream.clear();
		file.close();
	};

	// Insertion 
	benchmark(insertionLambda, Random(seed), iterations, size);
	log("insertion_random.csv");
	benchmark(insertionLambda, MonoGrowing(seed), iterations, size);
	log("insertion_monogrowing.csv");
	benchmark(insertionLambda, MonoFalling(seed), iterations, size);
	log("insertion_monofalling.csv");
	benchmark(insertionLambda, Constant(seed), iterations, size);
	log("insertion_constant.csv");

	// Selection 
	benchmark(selectionLambda, Random(seed), iterations, size);
	log("selection_random.csv");
	benchmark(selectionLambda, MonoGrowing(seed), iterations, size);
	log("selection_monogrowing.csv");
	benchmark(selectionLambda, MonoFalling(seed), iterations, size);
	log("selection_monofalling.csv");
	benchmark(selectionLambda, Constant(seed), iterations, size);
	log("selection_constant.csv");

	// Partition (Right) 
	benchmark(partitionLambda, Random(seed), iterations, size);
	log("partition_random.csv");
	benchmark(partitionLambda, MonoGrowing(seed), iterations, size);
	log("partition_monogrowing.csv");
	benchmark(partitionLambda, MonoFalling(seed), iterations, size);
	log("partition_monofalling.csv");
	benchmark(partitionLambda, Constant(seed), iterations, size);	//Missing data from here and below
	log("partition_constant.csv");

	// Partition (Median) 
	benchmark(medianPartitionLambda, Random(seed), iterations, size);
	log("medianPartition_random.csv");
	benchmark(medianPartitionLambda, MonoGrowing(seed), iterations, size);
	log("medianPartition_monogrowing.csv");
	benchmark(medianPartitionLambda, MonoFalling(seed), iterations, size);
	log("medianPartition_monofalling.csv");
	benchmark(medianPartitionLambda, Constant(seed), iterations, size);
	log("medianPartition_constant.csv");

	/* std::sort */
	benchmark(stdsortLambda, Random(seed), iterations, size);
	log("stdsort_random.csv");
	benchmark(stdsortLambda, MonoGrowing(seed), iterations, size);
	log("stdsort_monogrowing.csv");
	benchmark(stdsortLambda, MonoFalling(seed), iterations, size);
	log("stdsort_monofalling.csv");
	benchmark(stdsortLambda, Constant(seed), iterations, size);
	log("stdsort_constant.csv");


	std::cout.rdbuf(oldBuff);

	return 0;
}
