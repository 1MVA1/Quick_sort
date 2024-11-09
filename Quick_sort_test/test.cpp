#include "pch.h"
#include <gtest/gtest.h>
#include "../Quick_sort/Quick_sort.h"
#include <vector>

using namespace std;

/*
    ASSERT_EQ(expected, actual) � ��������� ���������.
    ASSERT_NE(expected, actual) � ���������, ��� �������� ������.
    ASSERT_TRUE(expression) � ���������, ��� ��������� �������.
    ASSERT_FALSE(expression) � ���������, ��� ��������� �����.
    EXPECT_* � ���������� ASSERT_*, �� ��������� ����� ���������� ����������, ���� ���� ���� �� ����������� �� ���������.
*/

// ���������� ������� ������� � ������� �� ������ ��������
TEST(No_need_sort_arrays) 
{
    vector<int> empy_arr;
    vector<int> arr = { 1 };

    fast_sort(empy_arr.data(), empy_arr.data() + empy_arr.size(), [](int a, int b) { return a < b; });
    fast_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });

    EXPECT_TRUE(empy_arr.empty());
    EXPECT_EQ(arr, vector<int>({ 1 }));
}

// ���������� ������� �� ���� ���������
TEST(Two_elem_array) 
{
    vector<int> arr = { 2, 1 };

    fast_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });

    EXPECT_EQ(arr, vector<int>({ 1, 2 }));
}

// ���������� ������� � �������������� �������
TEST(Array_with_negative_elems) 
{
    vector<int> arr = { 3, -1, 2, -5, 0 };

    fast_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });

    EXPECT_EQ(arr, vector<int>({ -5, -1, 0, 2, 3 }));
}

// ���������� �������������� ������� � �������������� � �������� �������
TEST(Sorted_arrays) {
    vector<int> sorted_arr = { 1, 2, 3, 4, 5 };
    vector<int> reverse_sorted_arr = { 5, 4, 3, 2, 1 };

    fast_sort(sorted_arr.data(), sorted_arr.data() + sorted_arr.size(), [](int a, int b) { return a < b; });
    fast_sort(reverse_sorted_arr.data(), reverse_sorted_arr.data() + reverse_sorted_arr.size(), [](int a, int b) { return a < b; });

    EXPECT_EQ(sorted_arr, vector<int>({ 1, 2, 3, 4, 5 }));
    EXPECT_EQ(reverse_sorted_arr, vector<int>({ 1, 2, 3, 4, 5 }));
}

// ���������� ������� � �������������� ����������.
TEST(Array_with_dublicates) 
{
    vector<int> arr = { 3, 1, 2, 3, 2, 1 };

    fast_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });

    EXPECT_EQ(arr, vector<int>({ 1, 1, 2, 2, 3, 3 }));
}

// ���������� ������� � 10000 ����������.
TEST(Array_with_10000_elems) 
{
    constexpr int N = 10000;
    vector<int> arr(N);

    // ��������� ������ � �������� �������
    for (int i = 0; i < N; ++i) {
        arr[i] = N - i; 
    }

    fast_sort(arr.data(), arr.data() + arr.size(), [](int a, int b) { return a < b; });

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

// ���������� ������� � ���������� ����������������� ����
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

    fast_sort(arr.data(), arr.data() + arr.size(), [](const Custom& a, const Custom& b) { return a < b; });

    EXPECT_EQ(arr[0].key, 1);
    EXPECT_EQ(arr[1].key, 2);
    EXPECT_EQ(arr[2].key, 2);

    EXPECT_EQ(arr[0].value, 1.1);
    EXPECT_EQ(arr[1].value, 1.2);
    EXPECT_EQ(arr[2].value, 2.1);
}