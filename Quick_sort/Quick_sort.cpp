﻿#include <iostream>
#include <vector>
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

vector<int> generate_random_vector(int size, int random_number)
{
    mt19937 gen(random_number);
    uniform_int_distribution<int> dist(0, 100000); // Распределение для генерации чисел от 0 до 100000
    vector<int> v(size);

    // Заполняем вектор случайными числами, используя лямбда-функцию
    generate(v.begin(), v.end(), [&]() { return dist(gen); });

    return v;
}

vector<int> generate_reverse_sorted_vector(int size)
{
    vector<int> v(size);

    // Заполняем вектор числами от size - 1 до 0 (обратный порядок)
    iota(v.rbegin(), v.rend(), 0);

    return v;
}

// Функция для измерения времени выполнения
template <typename Func>
double measure_time(Func&& func) 
{
    auto start = high_resolution_clock::now();

    func();

    auto end = high_resolution_clock::now();

    return duration<double>(end - start).count();
}

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

void chrono_tests()
{
    vector<int> array_sizes;
    vector<double> random_insertion_sort_times;
    vector<double> random_quick_sort_times;
    vector<double> reverse_insertion_sort_times;
    vector<double> reverse_quick_sort_times;

    random_device rd;           // Устройство для генерации случайного числа
    int random_numb = rd();     // Случайное число для инициализации генератора

    int tries = 100;
    double min_time;

    vector<int> vec;

    for (int i = 50; i <= 150; i += 1) {
        array_sizes.push_back(i);
    }

    for (int size : array_sizes)
    {
        cout << "Size = " << size << "\n";


        min_time = numeric_limits<double>::max();

        for (int i = 0; i < tries; ++i) {
            vec = generate_random_vector(size, random_numb);
            min_time = min(min_time, measure_time([&]() {insertion_sort(vec.data(), vec.data() + vec.size(), less<int>()); }));
        }

        random_insertion_sort_times.push_back(min_time);


        min_time = numeric_limits<double>::max();

        for (int i = 0; i < tries; ++i) {
            vec = generate_random_vector(size, random_numb);
            min_time = min(min_time, measure_time([&]() {quick_sort(vec.data(), vec.data() + vec.size(), less<int>()); }));
        }

        random_quick_sort_times.push_back(min_time);


        min_time = numeric_limits<double>::max();

        for (int i = 0; i < tries; ++i) {
            vec = generate_reverse_sorted_vector(size);
            min_time = min(min_time, measure_time([&]() {insertion_sort(vec.data(), vec.data() + vec.size(), less<int>()); }));
        }

        reverse_insertion_sort_times.push_back(min_time);


        min_time = numeric_limits<double>::max();

        for (int i = 0; i < tries; ++i) {
            vec = generate_reverse_sorted_vector(size);
            min_time = min(min_time, measure_time([&]() {quick_sort(vec.data(), vec.data() + vec.size(), less<int>()); }));
        }

        reverse_quick_sort_times.push_back(min_time);
    }

    save_results_to_csv(array_sizes, random_insertion_sort_times, random_quick_sort_times, reverse_insertion_sort_times, reverse_quick_sort_times);
}

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

    ankerl::nanobench::Bench bench;

    for (int i = 50; i <= 150; i += 1) {
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
        bench = ankerl::nanobench::Bench().warmup(50).minEpochIterations(200);

        // Запускаем измерение времени для функции быстрой сортировки
        bench.run("Random Array | insertion_sort", [&]() {insertion_sort(vec.data(), vec.data() + vec.size(), less<int>()); });

        // Получаем медианное время выполнения в наносекундах
        random_insertion_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));


        vec = generate_random_vector(size, random_numb + size);

        bench = ankerl::nanobench::Bench().warmup(50).minEpochIterations(200);

        bench.run("Random Array | quick_sort", [&]() {classic_quick_sort(vec.data(), vec.data() + vec.size(), less<int>()); });

        random_quick_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));


        vec = generate_reverse_sorted_vector(size);

        bench = ankerl::nanobench::Bench().warmup(20).minEpochIterations(30);

        bench.run("Reverse Array | insertion_sort ", [&]() {insertion_sort(vec.data(), vec.data() + vec.size(), less<int>()); });

        reverse_insertion_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));


        vec = generate_reverse_sorted_vector(size);

        bench = ankerl::nanobench::Bench().warmup(20).minEpochIterations(30);

        bench.run("Reverse Array | quick_sort", [&]() {classic_quick_sort(vec.data(), vec.data() + vec.size(), less<int>()); });

        reverse_quick_sort_times.push_back(bench.results().front().median(ankerl::nanobench::Result::Measure::elapsed));

    }

    save_results_to_csv(array_sizes, random_insertion_sort_times, random_quick_sort_times, reverse_insertion_sort_times, reverse_quick_sort_times);

}

int main()
{
    chrono_tests();

    //nanobench_tests();

    return 0;
}
