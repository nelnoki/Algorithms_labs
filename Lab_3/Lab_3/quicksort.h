#pragma once
#include <vector>
template <typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp);

template <typename T, typename Compare>
T* median(T* a, T* b, T* c, Compare comp);

template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp);

template <typename T, typename Compare>
void quicksort_no_insertion(T* first, T* last, Compare comp);

template <typename Func>
double measure_time(Func sort_function, std::vector<int> data);

std::vector<int> generate_random_data(size_t size);
std::vector<int> generate_reverse_sorted_data(size_t size);