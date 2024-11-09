#pragma once

#include <algorithm> // ��� swap
#include <iterator>  // ��� distance

using namespace std;

template<typename T, typename Comp>
T select_reference_element_as_median(T* first, T* mid, T* last, Comp comp) 
{
    if (comp(*mid, *first)) { 
        swap(*mid, *first);
    }
    if (comp(*last, *mid)) {
        swap(*last, *mid);
    }
    if (comp(*last, *first)) {
        swap(*last, *first);
    }

    return *mid;
}

template<typename T, typename Comp>
void insertion_sort(T* first, T* last, Comp comp)
{
    for (T* i = first + 1; i < last; ++i)
    {
        T value = move(*i);
        T* helper = i;

        // �������� �������� ������, ���� �� ����� ����� ��� ������� ��������
        while (helper > first && comp(value, *(helper - 1)))
        {
            *helper = move(*(helper - 1));
            --helper;
        }

        // ��������� ���������� ������� �� ���������� �����
        *helper = move(value);
    }
}

template<typename T, typename Comp>
void fast_sort(T* first, T* last, Comp comp) 
{
    // �������, ��� ������� ������������ ���������� ���������
    constexpr int treshold = 10;

    while (distance(first, last) > 1) 
    {
        int size = distance(first, last);

        if (size <= treshold) 
        {
            insertion_sort(first, last, comp);
            return;
        }

        T reference_element = select_reference_element_as_median(first, first + size / 2, last - 1, comp);

        T* left = first;
        T* right = last - 1;

        while (true) 
        {
            // ���������, �������� �� ������� ������� ������ �������� ��������.
            // ���� ������� �����������, �� ������� ��� ��������� �� ���� �����.
            // ���� ���������������, ����� ������� �������, ������� ������ ���� ������.
            while (comp(*left, reference_element)) {
                ++left;
            }

            // ���������� ������������ ����
            while (comp(reference_element, *right)) {
                --right; 
            }

           // ���� ��������� left � right ����������� ��� �����������, �� ����� �� left � ������ ��� ����� �������� ��������
           // � c����� �� right � ������ ��� ����� �������� ��������
            if (left >= right) {
                break;
            }

            // ������ ������� ��������, ������� ��������� "�� �� ����� ������"
            swap(*left, *right);

            // ������ �����:
            // 1) ���������� ��� ������������ ��������.
            // 2) ����������� ��������� � ��������� ���������, ������� ����� ���������.
            ++left;
            --right;
        }

        //����� ���������� ����� left ��������� �� ������ ������� ������� ����������.
        // ����� �������: [first, mid) � ��������� ���������, ������� ��� ������ ��������.
        // [mid, last) � ��������� ���������, ������� ��� ������ ��������.
        T* mid = left;

        // ��������� ������� �������� ���������� � ���������� ���������� ����������� ������� ��������
        if (distance(first, mid) < distance(mid, last)) 
        {
            sort(first, mid, comp);
            first = mid;
        }
        else 
        {
            sort(mid, last, comp);
            last = mid;
        }
    }
}