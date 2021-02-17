#include <iostream>
#include <chrono>
#include <iomanip>
#include "mat.hpp"
#include "tpool.hpp"

using namespace std::chrono;

static uint32_t TimeNow = system_clock::now().time_since_epoch().count(); // Fastest way to get epoch time

int main()
{
    static ThreadPool engine;
    static Mat<uint_fast8_t> data(10000, 10000);
    high_resolution_clock::time_point TimePointStart = high_resolution_clock::now();
    asyncfor_each(
        data.begin(), data.end(), [](uint_fast8_t &x) { x = ((system_clock::now().time_since_epoch().count() - TimeNow) * 48271) % 256; }, engine);
    high_resolution_clock::time_point TimePointEnd = high_resolution_clock::now();
    std::cout << std::setprecision(5) << std::fixed << "Runtime done: " << duration_cast<duration<double>>(TimePointEnd - TimePointStart).count() << '\n';
    return 0;
}
