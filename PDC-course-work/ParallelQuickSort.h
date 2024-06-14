#pragma once

#include <vector>
#include <iostream>
#include <oneapi/tbb.h>
#include "SequentialQuickSort.h"

template <typename T, int PL = 100>
void parallelQuickSortUtil(std::vector<T>& v, int left, int right, tbb::task_group& tg) {
    int i = left;
    int j = right;

    partition(v, i, j);

    if (left < j) {
        if (j - left < PL) {
            seqQuickSortUtil(v, left, j);
        }
        else {
            tg.run([&v, left, j, &tg] { parallelQuickSortUtil<T, PL>(v, left, j, tg); });
        }
    }
    if (i < right) {
        if (right - i < PL) {
            seqQuickSortUtil(v, i, right);
        }
        else {
            tg.run([&v, i, right, &tg] { parallelQuickSortUtil<T, PL>(v, i, right, tg); });
        }
    }
}

template <typename T, int PL = 100>
void parallelQuickSort(std::vector<T>& v) {
    tbb::task_group tg;
    parallelQuickSortUtil<T, PL>(v, 0, v.size() - 1, tg);
    tg.wait();
}