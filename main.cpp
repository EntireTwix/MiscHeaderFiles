#include <iostream>
#include <windows.h>
#include <random>
#include <chrono>
#include <iomanip>
#include <immintrin.h>
#include "tpool.h"
#include "mat.h"

using namespace std::chrono;

uint32_t TimeNow = system_clock::now().time_since_epoch().count(); // Fastest way to get epoch time

auto logical_cores = std::thread::hardware_concurrency();
auto size = 10000;
auto increment = size / logical_cores;
auto overflow = size % logical_cores;

void Random16(unsigned char *pAt, __m256i &timeValues, __m256i &constantValues, __m256i &res)
{
    for (int i = 0; i < 4; ++i)
    {
        timeValues[i] = (system_clock::now().time_since_epoch().count() - TimeNow); //4 x 16 bit values
    }

    constantValues = _mm256_set1_epi64x(48271);
    res = _mm256_mul_epi32(timeValues, constantValues);
    for (int i = 0; i < 4; ++i)
    {
        pAt[i] = res[i];
    }
}

int main()
{
    ThreadPool pool;
    double sum = 0;

    Mat<uint_fast8_t> a(size, size);

    for (int z = 0; z < 100; ++z)
    {
        pool.Pause();
        high_resolution_clock::time_point TimePointStart = high_resolution_clock::now();
        for (size_t i = 0; i < size - overflow; i += increment)
        {
            pool.AddTask([&a, i]() {
                __m256i t, c, r;

                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = i; k < i + increment; k += 4)
                    {
                        if ((k + 4) < size)
                            Random16(a.AtP(k, j), t, c, r);
                    }
                }
                //std::cout<<"finished "<<i<<'\n';
            });
        }
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
        //std::cout << a;
    }
    std::cout << (sum / 100) * -1 << '\n';
    std::cin.get();
    return 0;
}
