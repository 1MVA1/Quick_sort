#pragma once

#include <algorithm> // для swap
#include <iterator>  // для distance

using namespace std;

template<typename T, typename Comp>
T& select_reference_element_as_median(T* first, T* mid, T* last, Comp comp)
{
    if (comp(*mid, *first)) 
    {
        T helper = move(*mid);
        *mid = move(*first);
        *first = move(helper);
    }

    if (comp(*last, *mid)) 
    {
        T helper = move(*last);
        *last = move(*mid);
        *mid = move(helper);
    }

    if (comp(*last, *first)) 
    {
        T helper = move(*last);
        *last = move(*first);
        *first = move(helper);
    }

    return *mid;
}

// Наихудший случай для сортировки вставками — это массив, отсортированный в обратном порядке. 
// В этом случае для каждого элемента нужно выполнить наибольшее количество операций по сдвигу элементов. 
// Тестирование на таком массиве позволяет выявить худшую производительность, что важно для выбора оптимального порога при гибридной сортировке.
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

// Функция для вычисления коэффициента нарушений убывающего порядка
template<typename T, typename Comp>
double violation_coeff(T* first, T* last, Comp comp) 
{
    int violations = 0;

    for (T* it = first + 1; it < last; ++it) {
        if (comp(*(it - 1), *it)) {  // Если порядок убывания нарушен
            ++violations;
        }
    }

    return static_cast<double>(violations) / (distance(first, last) - 1);
}

template<typename T, typename Comp>
void quick_sort(T* first, T* last, Comp comp) 
{
    // Граница, при которой используется сортировка вставками
    constexpr int treshold = 30;

    while (distance(first, last) > 1) 
    {
        int size = distance(first, last);
        
        if (size <= treshold)
        {
            // Запускать одно из двух
            if (violation_coeff(first, last, comp) > 0.1)
            {
                insertion_sort(first, last, comp);
                return;
            }

            //insertion_sort(first, last, comp);
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
            T helper = move(*left);
            *left = move(*right);
            *right = move(helper);

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
            quick_sort(first, mid, comp);
            first = mid;
        }
        else 
        {
            quick_sort(mid, last, comp);
            last = mid;
        }
    }
}

template<typename T, typename Comp>
void classic_quick_sort(T* first, T* last, Comp comp)
{
    while (distance(first, last) > 1)
    {
        int size = distance(first, last);

        T reference_element = select_reference_element_as_median(first, first + size / 2, last - 1, comp);

        T* left = first;
        T* right = last - 1;

        while (true)
        {
            while (comp(*left, reference_element)) {
                ++left;
            }

            while (comp(reference_element, *right)) {
                --right;
            }

            if (left >= right) {
                break;
            }

            T helper = move(*left);
            *left = move(*right);
            *right = move(helper);

            ++left;
            --right;
        }

        T* mid = left;

        if (distance(first, mid) < distance(mid, last))
        {
            classic_quick_sort(first, mid, comp);
            first = mid;
        }
        else
        {
            classic_quick_sort(mid, last, comp);
            last = mid;
        }
    }
}