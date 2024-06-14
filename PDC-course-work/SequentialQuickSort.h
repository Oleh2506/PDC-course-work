#pragma once

#include <vector>

template <typename T>
T getMedianOfThree(const T& a, const T& b, const T& c) {
	if ((a < b && b < c) || (c < b && b < a)) return b;
	if ((b < a && a < c) || (c < a && a < b)) return a;
	return c;
}

template <typename T>
T getPivot(std::vector<T>& v, int left, int right)
{
	return getMedianOfThree(v[left], v[(left + right) / 2], v[right]);
}

template <typename T>
void partition(std::vector<T>& v, int& i, int& j)
{
	T pivot = getPivot(v, i, j);

	while (i <= j)
	{
		while (v[i] < pivot)
			i++;
		while (v[j] > pivot)
			j--;
		if (i <= j)
		{
			std::swap(v[i], v[j]);
			i++;
			j--;
		}
	}
}

template <typename T>
void seqQuickSortUtil(std::vector<T>& v, int left, int right)
{
	int i = left;
	int j = right;

	partition(v, i, j);

	if (left < j)
		seqQuickSortUtil(v, left, j);
	if (i < right)
		seqQuickSortUtil(v, i, right);
}

template <typename T>
void seqQuickSort(std::vector<T>& v)
{
	seqQuickSortUtil(v, 0, v.size() - 1);
}