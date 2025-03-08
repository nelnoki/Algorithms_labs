#include "quicksort.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <utility>


int INSERTION_SORT_THRESHOLD = 18; // Порог для использования сортировки вставками

// Сортировка вставками
template <typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i < last; ++i) { // Начинаем со 2 элемента
        T tmp = std::move(*i); // Сохраняем значение
        T* j = i; 
        while (j > first && comp(tmp, *(j - 1))) { // Пока j не дошел до первого элемента и пока tmp меньше предыдущего,
            *j = std::move(*(j - 1)); // Сдвигаем элементы вправо
            --j;
        }
        *j = std::move(tmp); 
    }
}

// Нахождение медианы
template <typename T, typename Compare>
T* median(T* a, T* b, T* c, Compare comp) {
    if (comp(*a, *b)) // Если a < b, проверяем где находится с - слева b или справа
        if (comp(*b, *c))
            return b;
        else
            if (comp(*a, *c)) // Если с слева, проверяем, левее а или правее
                return c;
            else
                return a;
    else
        if (comp(*a, *c)) // Аналогично, если а правее b, проверяем, с левее а или правее
            return a;
        else
            if (comp(*b, *c)) // Если с слева, проверяем, левее b или правее
                return c;
            else
                return b;
}

// Быстрая сортировка c модификациями
template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    while (last - first > INSERTION_SORT_THRESHOLD) { // Если массив небольшой, используем сортировку вставками
        T* m = median(first, first + (last - first) / 2, last - 1, comp); // Находим опорную точку
        std::swap(*m, *(last - 1)); 
        m = last - 1; // Меняем m с последним элементом

        T* i = first;
        T* j = last - 2;

        while (true) {
            while (comp(*i, *m)) ++i; // i двигается вправо, пока он меньше m
            while (comp(*m, *j)) --j; // j двигается влево, пока он больше m
            if (i >= j) break; 
            std::swap(*i, *j); 
            ++i; 
            --j;
        }
        std::swap(*i, *m);

        if (i - first < last - (i + 1)) { // Находим наименьший интервал и на нем рекурсивно выполянем быструю сортировку
            sort(first, i, comp);
            first = i + 1;
        }
        else {
            sort(i + 1, last, comp);
            last = i;
        }
    }

    insertion_sort(first, last, comp);
}

//Быстрая сортировка без сортировки вставками
template <typename T, typename Compare>
void quicksort_no_insertion(T* first, T* last, Compare comp) {
    if (last - first <= 1) return;

    T* m = first + (last - first) / 2;
    std::swap(*m, *(last - 1));
    m = last - 1;

    T* i = first;
    T* j = last - 2;

    while (true) {
        while (comp(*i, *m)) ++i;
        while (comp(*m, *j)) --j;
        if (i >= j) break;
        std::swap(*i, *j);
        ++i;
        --j;
    }
    std::swap(*i, *m);

    quicksort_no_insertion(first, i, comp);
    quicksort_no_insertion(i + 1, last, comp);
}

//Функция для измерения времени
template <typename Func>
double measure_time(Func sort_function, std::vector<int> data) {
    auto start = std::chrono::high_resolution_clock::now();
    sort_function(data);
    auto end = std::chrono::high_resolution_clock::now();
    return (std::chrono::duration<double>(end - start).count() * 1e9);
}

// Генерация массивов
std::vector<int> generate_random_data(size_t size) {
    std::vector<int> data(size);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> res(1, 10000);
    for (auto& x : data) {
        x = res(gen);
    }
    return data;
}

//Генерация обратно отсортированного массива
std::vector<int> generate_reverse_sorted_data(size_t size) {
    std::vector<int> data(size);
    for (size_t i = 0; i < size; ++i) data[i] = size - i;
    return data;
}

int main()
{
    std::vector<size_t> sizes /*= { 2, 4, 8, 16, 32, 64, 100, 1000, 10000 }*/;
    for(size_t i = 0; i <= 100; ++i) {
        sizes.push_back(i);
    }
    std::vector<double> quick_with_insertion, quick_without_insertion;
    for (size_t size : sizes) {
        auto random_data = generate_random_data(size);
        quick_with_insertion.push_back(measure_time([&](std::vector<int> data) { sort(data.data(), data.data() + data.size(), std::less<int>()); }, random_data));
        quick_without_insertion.push_back(measure_time([&](std::vector<int> data) { quicksort_no_insertion(data.data(), data.data() + data.size(), std::less<int>()); }, random_data));
    }

    std::vector<double> r_quick_with_insertion, r_quick_without_insertion;
    for (size_t size : sizes) {
        auto reversed_data = generate_reverse_sorted_data(size);
        r_quick_with_insertion.push_back(measure_time([&](std::vector<int> data) { sort(data.data(), data.data() + data.size(), std::less<int>()); }, reversed_data));
        r_quick_without_insertion.push_back(measure_time([&](std::vector<int> data) { quicksort_no_insertion(data.data(), data.data() + data.size(), std::less<int>()); }, reversed_data));
    }

    std::cout << "Size, QuickSort with Insertion, QuickSort without Insertion, QuickSort with Insertion on reversed, QuickSort without Insertion on reversed\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        std::cout << sizes[i] << ", " << quick_with_insertion[i] << ", " << quick_without_insertion[i] << ", " << r_quick_with_insertion[i] << ", " << r_quick_without_insertion[i] << "\n";
    }

    return 0; 
}

