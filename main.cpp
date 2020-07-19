#include <iostream>
#include <random>
#include <chrono>
#include "mat.h"
#include "tpool.h"

using namespace std::chrono;  

//std::minstd_rand LinearCongruential( ProgStartTime - system_clock::now().time_since_epoch().count() );
//a.At(i,j) = LinearCongruential();
//high_resolution_clock::time_point TimePointStart = high_resolution_clock::now();

uint32_t TimeNow = system_clock::now().time_since_epoch().count(); // Fastest way to get epoch time

#define logical_cores 1
#define size 10000
#define increment size/logical_cores

int main()
{
    ThreadPool pool(logical_cores);
    double sum = 0;

    Mat<size,size> a;

    for(size_t z = 0; z < 10; ++z)
    {
        pool.pause();
        high_resolution_clock::time_point TimePointStart = high_resolution_clock::now();
        for(size_t i = 0; i < size; i+=increment)
            pool.AddTask([&a,i](){
                for(size_t k = i; k < i+increment; ++k)
                for(size_t j = 0; j < size; ++j)
                {
                    std::minstd_rand LinearCongruential( TimeNow - system_clock::now().time_since_epoch().count() );
                    a.At(k,j) = LinearCongruential();
                }
            });
        high_resolution_clock::time_point TimePointEnd = high_resolution_clock::now();
        //std::cout<<pool.JobsLeft()<<" start jobs\n";
        //std::cout<<"Thread Task Compilation Time done: "<<duration_cast<duration<double>>(TimePointEnd - TimePointStart).count()<<'\n';

        TimePointStart = high_resolution_clock::now();
        pool.start();
        while(pool.JobsLeft()) { /*std::cout<<pool.JobsLeft()<<" tasks\n";*/ }
        TimePointEnd = high_resolution_clock::now();
        //std::cout<<pool.JobsLeft()<<" end jobs\n";

        std::cout<<"Runtime done: "<<duration_cast<duration<double>>(TimePointEnd - TimePointStart).count()<<"\n";
        //std::cout<<a;
        if(z>0) sum+=duration_cast<duration<double>>(TimePointEnd - TimePointStart).count();
    }
    std::cout<<sum/9<<'\n';
    return 0;
}
