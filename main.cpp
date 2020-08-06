#include <iostream>
#include <windows.h>
#include <random>
#include <chrono>
#include <iomanip>
#include "tpool.h"
#include "mat.h"

using namespace std::chrono;

uint32_t TimeNow = system_clock::now().time_since_epoch().count(); // Fastest way to get epoch time

auto logical_cores = 12;
auto size = 10000;
auto increment = size / logical_cores;
auto overflow = size%logical_cores;

int main()
{

    ThreadPool<12> pool;
    double sum = 0;

    Mat a(size, size);

    for (size_t z = 0; z < 100; ++z)
    {
        pool.Pause();
        high_resolution_clock::time_point TimePointStart = high_resolution_clock::now();
        for (size_t i = 0; i < size-overflow; i += increment)
            pool.AddTask([&a, i]() {
            //std::cout<<"working "<<i<<'\n';
            for (size_t j = 0; j < size; ++j)
                for (size_t k = i; k < i + increment; ++k)
                {
                    std::minstd_rand LinearCongruential(TimeNow - system_clock::now().time_since_epoch().count());

                    a.At(k, j) = LinearCongruential();
                }
            //std::cout<<"finished "<<i<<'\n';
                });
        high_resolution_clock::time_point TimePointEnd = high_resolution_clock::now();
        std::cout << pool.Jobs() << " start jobs\n";
        //std::cout<<"Thread Task Compilation Time done: "<<duration_cast<duration<double>>(TimePointEnd - TimePointStart).count()<<'\n';
        TimePointStart = high_resolution_clock::now();
        pool.Start();
        while (pool.Jobs())
        {
        }
        TimePointEnd = high_resolution_clock::now();
        std::cout << pool.Jobs() << " end jobs\n";

        std::cout << std::setprecision(5) << std::fixed << "Runtime done: " << duration_cast<duration<double>>(TimePointEnd - TimePointStart).count() << "\n\n";
        sum += duration_cast<duration<double>>(TimePointEnd - TimePointStart).count();
        //std::cout<<a;
    }
    std::cout << (sum / 100) * -1 << '\n';
    return 0;
}
