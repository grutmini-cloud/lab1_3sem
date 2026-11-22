#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "BinaryTree.h"

inline long long BenchmarkKey(int i) { // ключ номер i: выглядит случайным, но никогда не повторяется
    unsigned int value = (unsigned int)i;
    value = value ^ (value >> 16);
    value = value * 0x85ebca6bu;
    value = value ^ (value >> 13);
    value = value * 0xc2b2ae35u;
    value = value ^ (value >> 16);
    return (long long)value;
}

inline double ElapsedMs(std::chrono::steady_clock::time_point start) { // сколько миллисекунд прошло с start
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

inline void BenchmarkSize(int count, std::ofstream& file) { // замер вставки, поиска и удаления для count элементов
    BinaryTree<long long> tree;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++) {
        tree.Insert(BenchmarkKey(i));
    }
    double insertMs = ElapsedMs(start);
    int height = tree.GetHeight();

    int found = 0;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++) {
        if (tree.Contains(BenchmarkKey(i))) {
            found++;
        }
    }
    double searchMs = ElapsedMs(start);

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++) {
        tree.Remove(BenchmarkKey(i));
    }
    double removeMs = ElapsedMs(start);

    std::cout << std::setw(10) << count
              << std::setw(14) << insertMs
              << std::setw(14) << searchMs
              << std::setw(14) << removeMs
              << std::setw(8) << height << std::endl;
    if (found != count) {
        std::cout << "Warning: found " << found << " of " << count << std::endl;
    }

    file << count << "," << insertMs << "," << searchMs << "," << removeMs << "," << height << std::endl;
}

inline void RunBenchmark(int maxCount) { // замеры для 10^4, 10^5, ... до maxCount, результат в benchmark.csv
    std::ofstream file("benchmark.csv");
    file << "count,insert_ms,search_ms,remove_ms,height" << std::endl;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << std::setw(10) << "count"
              << std::setw(14) << "insert, ms"
              << std::setw(14) << "search, ms"
              << std::setw(14) << "remove, ms"
              << std::setw(8) << "height" << std::endl;

    for (int count = 10000; count <= maxCount; count *= 10) {
        BenchmarkSize(count, file);
    }

    std::cout << "Results saved to benchmark.csv" << std::endl;
}
