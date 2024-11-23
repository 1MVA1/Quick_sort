#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <numeric>
#include <fstream>
#include "Quick_sort.h"
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../nanobench/src/include/nanobench.h"

using namespace std;
using namespace chrono;

// Генерация случайного вектора
vector<int> generate_random_vector(int size, int random_number)
{
    mt19937 gen(random_number);
    uniform_int_distribution<int> dist(0, 100000); // Распределение для генерации чисел от 0 до 100000
    vector<int> v(size);

    // Заполняем вектор случайными числами, используя лямбда-функцию
    generate(v.begin(), v.end(), [&]() { return dist(gen); });

    return v;
}

// Генерация вектора обратноотсортированного
vector<int> generate_reverse_sorted_vector(int size)
{
    vector<int> v(size);

    // Заполняем вектор числами от size - 1 до 0 (обратный порядок)
    iota(v.rbegin(), v.rend(), 0);

    return v;
}

// Функция для измерения времени выполнения с #include <chrono>
template <typename Func>
double measure_time(Func&& func) 
{
    auto start = high_resolution_clock::now();

    func();

    auto end = high_resolution_clock::now();

    return duration<double>(end - start).count();
}

// Сохранение результатов
void save_results_to_csv(const vector<int>& sizes, const vector<double>& rand_is, 
    const vector<double>& rand_qs, const vector<double>& rev_is, const vector<double>& rev_qs)
{
    ofstream file("results.csv");

    if (!file.is_open())
    {
        cerr << "Cann't open results.csv\n";
        return;
    }

    // Заголовок CSV-файла
    file << "size,random_insertion_sort_times,random_quick_sort_times,reverse_insertion_sort_times,reverse_quick_sort_times\n";

    for (size_t i = 0; i < sizes.size(); ++i) {
        file << sizes[i] << "," << rand_is[i] << "," << rand_qs[i] << "," << rev_is[i] << "," << rev_qs[i] << "\n";
    }

    file.close();
}

// Вспомогательые функции запуска тестов
void insertion_sort_test(vector<int> vec)
{
    vector<int> vec_copy = vec;

    insertion_sort(vec_copy.data(), vec_copy.data() + vec_copy.size(), less<int>());
}

void quick_sort_test(vector<int> vec)
{
    vector<int> vec_copy = vec;

    //  Запускать одно из двух
    //classic_quick_sort(vec_copy.data(), vec_copy.data() + vec_copy.size(), less<int>());
    quick_sort(vec.data(), vec.data() + vec.size(), less<int>());
}

// Запуск тестов с помощью #include <chrono>
void chrono_tests()
{
    vector<int> array_sizes;
    vector<double> random_insertion_sort_times;
    vector<double> random_quick_sort_times;
    vector<double> reverse_insertion_sort_times;
    vector<double> reverse_quick_sort_times;

    random_device rd;           // Устройство для генерации случайного числа
    int random_numb = rd();     // Случайное число для инициализации генератора

    int tries = 10000;
    double time;

    vector<int> vec;

    for (int i = 2; i <= 60; i += 1) {
        array_sizes.push_back(i);
    }

    for (int size : array_sizes)
    {
        cout << "Size = " << size << "\n";

        vec = generate_random_vector(size, random_numb);


        time = 0;

        for (int i = 0; i < tries; ++i) {
            time += measure_time([&]() { insertion_sort_test(vec); });
        }

        random_insertion_sort_times.push_back(time / tries);


        time = 0;

        for (int i = 0; i < tries; ++i) {
            time += measure_time([&]() { quick_sort_test(vec); });
        }

        random_quick_sort_times.push_back(time / tries);


        vec = generate_reverse_sorted_vector(size);


        time = 0;

        for (int i = 0; i < tries; ++i) {
            time += measure_time([&]() { insertion_sort_test(vec); });
        }

        reverse_insertion_sort_times.push_back(time / tries);


        time = 0;

        for (int i = 0; i < tries; ++i) {
            time += measure_time([&]() {quick_sort_test(vec); });
        }

        reverse_quick_sort_times.push_back(time / tries);
    }

    save_results_to_csv(array_sizes, random_insertion_sort_times, random_quick_sort_times, reverse_insertion_sort_times, reverse_quick_sort_times);
}

// Запуск тестов с помощью #include "../nanobench/src/include/nanobench.h"
void nanobench_tests()
{
    vector<int> array_sizes;
    vector<double> random_insertion_sort_times;
    vector<double> random_quick_sort_times;
    vector<double> reverse_insertion_sort_times;
    vector<double> reverse_quick_sort_times;

    random_device rd;
    int random_numb = rd();

    vector<int> vec;
    int warmups = 1000, epochs = 10000;

    ankerl::nanobench::Bench bench;

    for (int i = 2; i <= 60; i += 1) {
        array_sizes.push_back(i);
    }

    for (int size : array_sizes)
    {
        cout << "Size = " << size << "\n";


        vec = generate_random_vector(size, random_numb + size);


        // Создаём объект для измерения времени
        // Конструктор Bench() создаёт объект для выполнения бенчмарков
        // warmup() задаёт количество прогревочных запусков
        // minEpochIterations() устанавливает минимальное количество запусков тестируемой функции в одной "эпохе".
        // Эпоха — это набор запусков функции, за которые измеряется время выполнения.
        bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);

        // Запускаем измерение времени для функции быстрой сортировки
        bench.run("Random array | Insertion sort", [&]() {insertion_sort_test(vec); });

        // Получаем медианное время выполнения в наносекундах
        random_insertion_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));



        bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);

        bench.run("Random array | Quick sort", [&]() {quick_sort_test(vec); });

        random_quick_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));


        vec = generate_reverse_sorted_vector(size);


        bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);

        bench.run("Reverse array | Insertion sort", [&]() {insertion_sort_test(vec); });

        reverse_insertion_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));



        bench = ankerl::nanobench::Bench().warmup(warmups).minEpochIterations(epochs);

        bench.run("Reverse array | Quick sort", [&]() {quick_sort_test(vec); });

        reverse_quick_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));

    }

    save_results_to_csv(array_sizes, random_insertion_sort_times, random_quick_sort_times, reverse_insertion_sort_times, reverse_quick_sort_times);

}

int main()
{
    // Запускать одно из двух
    //chrono_tests();
    nanobench_tests();

    return 0;
}
