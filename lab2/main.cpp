#include "generators.hpp"
#include "stopwatch.hpp"
#include "sorts.hpp"

#include <iostream>
#include <vector>


template<typename Sort, typename Populate>
void benchmark(Sort sort, Populate populate, unsigned iterations, unsigned size)
{
	std::vector<int> vector(size);
	for(unsigned i = 0; i < iterations; i++)
	{
		std::generate(vector.begin(), vector.end(), populate);

		Stopwatch watch;
		sort(vector.begin(), vector.end() );

		std::cout << watch.getSeconds() << '\t';
	}
	std::cout << "\n\n";
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

	std::vector<int> vector(1 << 15);
	auto seed = std::random_device{}();

	unsigned iterations = 5;
	unsigned size = 1 << 15;

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

	return 0;
}
