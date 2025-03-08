#include "pch.h"
#include <gtest/gtest.h>
#include "../../Lab_3/Lab_3/quicksort.cpp"
#include <vector>

using namespace std;

// Сортировка пустого массива и массива из одного элемента
TEST(No_need_sort_arrays)
{
    vector<int> empy_arr;
    vector<int> arr = { 1 };

    sort(empy_arr.data(), empy_arr.data() + empy_arr.size(), less<int>());
    sort(arr.data(), arr.data() + arr.size(), less<int>());

    EXPECT_TRUE(empy_arr.empty());
    EXPECT_EQ(arr, vector<int>({ 1 }));
}

// Сортировка массива из двух элементов
TEST(Two_elem_array)
{
    vector<int> arr = { 2, 1 };

    sort(arr.data(), arr.data() + arr.size(), less<int>());

    EXPECT_EQ(arr, vector<int>({ 1, 2 }));
}

// Сортировка массива с отрицательными числами
TEST(Array_with_negative_elems)
{
    vector<int> arr = { 3, -1, 2, -5, 0 };

    sort(arr.data(), arr.data() + arr.size(), less<int>());

    EXPECT_EQ(arr, vector<int>({ -5, -1, 0, 2, 3 }));
}

// Сортировка упорядоченного массива и упорядоченного в обратном порядке
TEST(Sorted_arrays) {
    vector<int> sorted_arr = { 1, 2, 3, 4, 5 };
    vector<int> reverse_sorted_arr = { 5, 4, 3, 2, 1 };

    sort(sorted_arr.data(), sorted_arr.data() + sorted_arr.size(), less<int>());
    sort(reverse_sorted_arr.data(), reverse_sorted_arr.data() + reverse_sorted_arr.size(), less<int>());

    EXPECT_EQ(sorted_arr, vector<int>({ 1, 2, 3, 4, 5 }));
    EXPECT_EQ(reverse_sorted_arr, vector<int>({ 1, 2, 3, 4, 5 }));
}

// Сортировка массива с повторяющимися элементами.
TEST(Array_with_dublicates)
{
    vector<int> arr = { 3, 1, 2, 3, 2, 1 };

    sort(arr.data(), arr.data() + arr.size(), less<int>());

    EXPECT_EQ(arr, vector<int>({ 1, 1, 2, 2, 3, 3 }));
}

// Сортировка массива с 10000 элементами.
TEST(Array_with_10000_elems)
{
    constexpr int N = 10000;
    vector<int> arr(N);

    // Заполняем массив в обратном порядке
    for (int i = 0; i < N; ++i) {
        arr[i] = N - i;
    }

    sort(arr.data(), arr.data() + arr.size(), less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

// Сортировка массива с элементами пользовательского типа
struct Custom {
    int key;
    double value;

    bool operator < (const Custom& other) const {
        if (key == other.key) {
            return value < other.value;
        }

        return key < other.key;
    }
};

TEST(Custom_array)
{
    vector<Custom> arr = { {2, 2.1}, {2, 1.2}, {1, 1.1} };

    sort(arr.data(), arr.data() + arr.size(), [](const Custom& a, const Custom& b) { return a < b; });

    EXPECT_EQ(arr[0].key, 1);
    EXPECT_EQ(arr[1].key, 2);
    EXPECT_EQ(arr[2].key, 2);

    EXPECT_EQ(arr[0].value, 1.1);
    EXPECT_EQ(arr[1].value, 1.2);
    EXPECT_EQ(arr[2].value, 2.1);
}