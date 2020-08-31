#include <iostream>
#include <windows.h>
#include <random>
#include <chrono>
#include <iomanip>
#include <immintrin.h>
#include "tpool.h"
#include "mat.h"

using namespace std::chrono;

int main()
{
    std::cout << "input threads\n";
    int inp;
    std::cin >> inp;
    auto logical_cores = inp;
    auto size = 10000;
    auto increment = size / logical_cores;
    auto overflow = size % logical_cores;

    ThreadPool pool;
    double sum = 0;

    Mat<uint_fast8_t> a(size, size);

    high_resolution_clock::time_point TimePointStart,
        TimePointEnd;
    for (int z = 0; z < 100; ++z)
    {
        pool.Pause();
        for (size_t i = 0; i < size - overflow; i += increment)
        {
            pool.AddTask([&a, i, increment, size]() {
                size_t index = i;
                __m256i regi;
                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = i; k < i + increment; ++k)
                    {
                        //Ranadom64(a.At(index), index, regi);
                        (*a.At(index)) = ((index * 35060) + 11) % 256;

                        ++index;
                    }
                }
            });
        }

        TimePointStart = high_resolution_clock::now();
        pool.Start();
        while (pool.Jobs())
        {
        }
        TimePointEnd = high_resolution_clock::now();

        std::cout << std::setprecision(5) << std::fixed << "Runtime done: " << duration_cast<duration<double>>(TimePointEnd - TimePointStart).count() << "\n\n";
        sum += duration_cast<duration<double>>(TimePointEnd - TimePointStart).count();
    }
    std::cout << (sum / 100) * -1 << '\n';
    std::cin.get();
    return 0;
}
