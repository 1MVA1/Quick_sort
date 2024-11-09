#pragma once

#include <algorithm> // для swap
#include <iterator>  // для distance

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

        // Сдвигаем элементы вправо, пока не найдём место для вставки текущего
        while (helper > first && comp(value, *(helper - 1)))
        {
            *helper = move(*(helper - 1));
            --helper;
        }

        // Вставляем сохранённый элемент на правильное место
        *helper = move(value);
    }
}

template<typename T, typename Comp>
void fast_sort(T* first, T* last, Comp comp) 
{
    // Граница, при которой используется сортировка вставками
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
            // Проверяем, является ли текущий элемент меньше опорного элемента.
            // Если условие выполняется, то элемент уже находится на своём месте.
            // Цикл останавливается, когда находим элемент, который должен быть справа.
            while (comp(*left, reference_element)) {
                ++left;
            }

            // Аналогично рассуждениям выше
            while (comp(reference_element, *right)) {
                --right; 
            }

           // Если указатели left и right пересеклись или встретились, то слева от left — меньше или равны опорному элементу
           // и cправа от right — больше или равны опорному элементу
            if (left >= right) {
                break;
            }

            // Меняем местами элементы, которые находятся "не на своих местах"
            swap(*left, *right);

            // Теперь нужно:
            // 1) Пропустить уже обработанные элементы.
            // 2) Переместить указатели к следующим элементам, которые нужно проверить.
            ++left;
            --right;
        }

        //После завершения цикла left указывает на первый элемент правого подмассива.
        // Таким образом: [first, mid) — подмассив элементов, меньших или равных опорному.
        // [mid, last) — подмассив элементов, больших или равных опорному.
        T* mid = left;

        // Сортируем меньший интервал рекурсивно и продолжаем итеративно сортировать больший интервал
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