#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>       
#include <functional>

class ThreadPool
{
private:
    bool stopped = false, paused = true;

    std::mutex* threadLocks;
    std::condition_variable* jobListener;
    std::queue<std::function<void()> >* jobs; //a queue for each thread
    std::thread* workers;
    uint_fast8_t nextIndex = 0, threadCount;
public:
    ThreadPool(uint_fast8_t threads)
    {
        threadCount = threads;
        threadLocks = new std::mutex[threadCount];
        jobListener = new std::condition_variable[threadCount];
        jobs = new std::queue<std::function<void()> >[threadCount];
        workers = new std::thread[threadCount];

        for(uint_fast8_t i = 0; i < threadCount; ++i)
            workers[i] = std::thread([this,i](){
                    std::function<void()> job;
                    
                    while(!stopped)
                    {  
                        {
                            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
                            jobListener[i].wait(jobsAccess, [this,i](){ return !(jobs[i].empty() || stopped); } );
                        }

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
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[nextIndex]}; //when its this threads turn
            jobs[nextIndex].push(func);
        }
        jobListener[nextIndex].notify_one();
        
        nextIndex = nextIndex==(threadCount-1)?0:nextIndex+1;
    }
    size_t JobsLeft()
    {
        size_t sum = 0;
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            {
                std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
                sum += jobs[i].size();
            }
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
