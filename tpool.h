#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>       
#include <functional>

class ThreadPool final
{
private:
    bool stopped = false, paused = true;

    std::mutex* threadLocks; //lock for using jobs que
    std::condition_variable* jobListener; //listener for waiting for jobs update
    std::queue<std::function<void()> >* jobs; //a queue for each thread
    std::thread* workers; //threads
    uint_fast8_t threadCount;
public:
    ThreadPool(uint_fast8_t threads = 0)
    {
        threadCount = threads?threads:std::thread::hardware_concurrency();
        threadLocks = new std::mutex[threadCount];
        jobListener = new std::condition_variable[threadCount];
        jobs = new std::queue<std::function<void()> >[threadCount];
        workers = new std::thread[threadCount];

        for(uint_fast8_t i = 0; i < threadCount; ++i)
            workers[i] = std::thread([this,i](){
                    std::function<void()> job;
                    
                    while(1)
                    {  
                        {
                            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; 
                            jobListener[i].wait(jobsAccess, [this,i](){ return !jobs[i].empty() || stopped; } );
                        }

                        if(stopped) break;

                        if(!paused)
                        {
                            job = jobs[i].front();
                            job();
                            jobs[i].pop();
                        }
                    }
            });
    }

    
    void AddTask(std::function<void()> func)
    {
        //finding worker with least jobs
        size_t smallest = -1;
        uint_fast8_t index;
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            if(jobs[i].size()<smallest)
            {
                smallest = jobs[i].size();
                index = i;
            }
        }

        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[index]}; 
            jobs[index].push(func);
        }
        jobListener[index].notify_one();
    }
    size_t JobsLeft()
    {
        size_t sum = 0;
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; 
            sum += jobs[i].size();
        }
        return sum;
    }

    void start() 
    {
        paused = false;
    }
    void pause()
    {
        paused = true;
    }
    void stop()
    {
        stopped = true;
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            jobListener[i].notify_one();
        }
    }
    

    ~ThreadPool()
    {
        stop();
        for(size_t i = 0; i < threadCount; ++i)
            if(workers[i].joinable()) workers[i].join();
        delete[] threadLocks;
        delete[] jobListener;
        delete[] jobs;
        delete[] workers;
    }
};
