#include <iostream>
#include <ctime>
#include <chrono>
#include "mat.hpp"
#include "tpool.hpp"

using namespace std::chrono;

uint32_t TimeNow = system_clock::now().time_since_epoch().count();

int main()
{
    static ThreadPool engine;
    static Mat<uint_fast8_t> data(10000, 10000);
    while (1)
    {
        asyncfor_each(
            data.begin(), data.end(), [](uint_fast8_t &x) { x = ((system_clock::now().time_since_epoch().count() - TimeNow) * 48271) % 256; }, engine);
    }
    return 0;
}
