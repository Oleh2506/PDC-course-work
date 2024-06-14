#include <vector>
#include <algorithm>
#include "SortableItem.h"
#include "SequentialQuickSort.h"
#include "ParallelQuickSort.h"
#include <oneapi/tbb.h>
#include <chrono>
#include <random>
#include <thread>

void fillRandomSortableItemVec(std::vector<SortableItem>& v, int size) {
	for (int i = 0; i < size; i++)
		v.push_back(SortableItem::generateRandom());
}

void verifySeqQuickSort() {
	const int TEST_COUNT = 8;
	const int VEC_SIZES[TEST_COUNT] = { 50000, 100000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000 };

	for (int i = 0; i < TEST_COUNT; i++) {
		std::vector<SortableItem> v1;
		fillRandomSortableItemVec(v1, VEC_SIZES[i]);

		std::vector<SortableItem> v2 = v1;

		seqQuickSort(v1);
		std::sort(v2.begin(), v2.end());

		if (v1 == v2) {
			std::cout << "Sequential quick sort successfully sorted " << VEC_SIZES[i] << " elements, matching std::sort.\n";
		}
		else {
			std::cout << "Sequential quick sort failed to correctly sort " << VEC_SIZES[i] << " elements compared to std::sort.\n";
		}
	}
}

void verifyParallelQuickSort() {
	const int TEST_COUNT = 8;
	const int VEC_SIZES[TEST_COUNT] = { 50000, 100000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000 };

	int maxThreads = std::thread::hardware_concurrency();
	tbb::global_control control(tbb::global_control::max_allowed_parallelism, maxThreads);

	for (int i = 0; i < TEST_COUNT; i++) {
		std::vector<SortableItem> v1;
		fillRandomSortableItemVec(v1, VEC_SIZES[i]);

		std::vector<SortableItem> v2 = v1;

		parallelQuickSort(v1);
		std::sort(v2.begin(), v2.end());

		if (v1 == v2) {
			std::cout << "Parallel quick sort successfully sorted " << VEC_SIZES[i] << " elements, matching std::sort.\n";
		}
		else {
			std::cout << "Parallel quick sort failed to correctly sort " << VEC_SIZES[i] << " elements compared to std::sort.\n";
		}
	}
}

void testSeqQuickSortPerformance() {
	const int TEST_COUNT = 20;
	const int SIZE_COUNT = 8;
	const int VEC_SIZES[SIZE_COUNT] = { 50000, 100000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000 };

	std::cout << "Sequential quick sort performance testing\n";
	for (int sizeIndex = 0; sizeIndex < SIZE_COUNT; sizeIndex++) {
		std::vector<SortableItem> v;
		fillRandomSortableItemVec(v, VEC_SIZES[sizeIndex]);

		double totalDuration = 0.0;

		for (int testIndex = 0; testIndex < TEST_COUNT; testIndex++) {
			auto startTime = std::chrono::high_resolution_clock::now();
			seqQuickSort(v);
			auto endTime = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
			totalDuration += duration.count();

			std::random_device rd;
			std::mt19937 gen(rd());
			std::shuffle(v.begin(), v.end(), gen);
		}

		double meanDuration = totalDuration / TEST_COUNT;
		std::cout << std::fixed << "Mean time to sort " << VEC_SIZES[sizeIndex] << " elements: " << meanDuration << " microseconds.\n";
	}
}

template <int PL = 100>
void testParallelLimit() {
	const int TEST_COUNT = 20;
	const int SIZE_COUNT = 8;
	const int VEC_SIZES[SIZE_COUNT] = { 50000, 100000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000 };

	int maxThreads = std::thread::hardware_concurrency();
	tbb::global_control control(tbb::global_control::max_allowed_parallelism, maxThreads);

	std::cout << "Parallel limit: " << PL << ".\n";

	for (int sizeIndex = 0; sizeIndex < SIZE_COUNT; sizeIndex++) {
		std::vector<SortableItem> v;
		fillRandomSortableItemVec(v, VEC_SIZES[sizeIndex]);

		double totalDuration = 0.0;

		for (int testIndex = 0; testIndex < TEST_COUNT; testIndex++) {
			auto startTime = std::chrono::high_resolution_clock::now();
			parallelQuickSort<SortableItem, PL>(v);
			auto endTime = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
			totalDuration += duration.count();

			std::random_device rd;
			std::mt19937 gen(rd());
			std::shuffle(v.begin(), v.end(), gen);
		}

		double meanDuration = totalDuration / TEST_COUNT;
		std::cout << std::fixed << "Mean time to sort " << VEC_SIZES[sizeIndex] << " elements: " << meanDuration << " microseconds.\n";
	}
}

void testParallelQuickSortParallelLimits() {
	testParallelLimit<100>();
	std::cout << std::endl;

	testParallelLimit<1000>();
	std::cout << std::endl;

	testParallelLimit<10000>();
	std::cout << std::endl;

	testParallelLimit<50000>();
	std::cout << std::endl;
}

void testParallelQuickSortPerformance() {
	const int TEST_COUNT = 20;
	const int SIZE_COUNT = 8;
	const int THREADS_COUNT = 3;
	const int VEC_SIZES[SIZE_COUNT] = { 50000, 100000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000 };
	const int THREAD_COUNTS[THREADS_COUNT] = { 2, 4, 8 };

	std::cout << "Parallel quick sort performance testing\n";
	for (int threadIndex = 0; threadIndex < THREADS_COUNT; threadIndex++) {
		int numThreads = THREAD_COUNTS[threadIndex];
		tbb::global_control c(tbb::global_control::max_allowed_parallelism, numThreads);

		std::cout << "Thread Count: " << numThreads << ".\n";

		for (int sizeIndex = 0; sizeIndex < SIZE_COUNT; sizeIndex++) {
			std::vector<SortableItem> v;
			fillRandomSortableItemVec(v, VEC_SIZES[sizeIndex]);

			double totalDuration = 0.0;

			for (int testIndex = 0; testIndex < TEST_COUNT; testIndex++) {
				auto startTime = std::chrono::high_resolution_clock::now();
				parallelQuickSort(v);
				auto endTime = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
				totalDuration += duration.count();

				std::random_device rd;
				std::mt19937 gen(rd());
				std::shuffle(v.begin(), v.end(), gen);
			}

			double meanDuration = totalDuration / TEST_COUNT;
			std::cout << std::fixed << "Mean time to sort " << VEC_SIZES[sizeIndex] << " elements: " << meanDuration << " microseconds.\n";
		}

		std::cout << std::endl;
	}
}

std::vector<SortableItem> generateRandomVector(int size, const std::vector<SortableItem>& predefinedElements) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, predefinedElements.size() - 1);

	std::vector<SortableItem> randomVector;
	randomVector.reserve(size);

	for (int i = 0; i < size; ++i) {
		int randomIndex = dis(gen);
		randomVector.push_back(predefinedElements[randomIndex]);
	}

	return randomVector;
}

int main() {
	testParallelQuickSortPerformance();
	return 0;
}