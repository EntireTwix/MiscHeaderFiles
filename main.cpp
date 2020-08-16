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

//returns 4x 64bit random values
void Random64(unsigned char *pAt, __m256i &timeValues, const __m256i &constantValues, __m256i &res)
{

    for (int i = 0; i < 4; ++i)
    {
        timeValues[i] = (system_clock::now().time_since_epoch().count() - TimeNow); //4 x 64 bit values
    }

    res = _mm256_mul_epi32(timeValues, constantValues);
    for (int i = 0; i < 4; ++i)
    {
        pAt[i] = res[i] % 256;
    }
}

int main()
{
    ThreadPool pool;
    double sum = 0;

    Mat<uint_fast8_t> a(size, size);

    __m256i c = _mm256_set1_epi64x(48271);

    for (int z = 0; z < 100; ++z)
    {
        pool.Pause();
        high_resolution_clock::time_point TimePointStart = high_resolution_clock::now();
        for (size_t i = 0; i < size - overflow; i += increment)
        {
            pool.AddTask([&a, i, c]() {
                __m256i t, r;

                for (size_t j = 0; j < size; ++j)
                {
                    for (size_t k = i; k < i + increment; k += 4)
                    {
                        Random64(a.AtP(k, j), t, c, r);
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
