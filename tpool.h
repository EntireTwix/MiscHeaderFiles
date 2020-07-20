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
                    while(!stopped)
                    {                        
                    
                            std::function<void()> job;
                            //std::cout<<"Thread "<<(int)i+1<<" is running\n";
                            
                            {
                                std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
                                //if((!jobs[i].empty() || stopped)) std::cout<<"waiting\n";
                                jobListener[i].wait(jobsAccess, [this,i](){ return !(jobs[i].empty() || stopped); } );
                            }
                        
                            if(!paused)
                            {
                                //std::cout<<"working\n";
                                job = jobs[i].front();
                                job();
                                jobs[i].pop();
                            }
                        
                        //else std::cout<<"Thread "<<(int)i+1<<" is paused\n";
                    }
            });

        //std::cout<<(int)threadCount<<" threads spawned\n";
    }

    
    void AddTask(std::function<void()> func)
    {
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[nextIndex]}; //when its this threads turn
            jobs[nextIndex].push(func);
            jobListener[nextIndex].notify_one();
            //std::cout<<"jobs notified "<<nextIndex<<'\n';
            //std::cout<<"job added\n";
        }
        nextIndex = nextIndex==(threadCount-1)?0:nextIndex+1;
    }
    size_t JobsLeft()
    {
        size_t sum = 0;
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
            sum += jobs[i].size();
        }
        return sum;
    }

    void start() 
    {
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
            paused = false;
        }
    }
    void pause()
    {
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
            paused = true;
        }
    }
    void stop()
    {
        for(uint_fast8_t i = 0; i < threadCount; ++i)
        {
            std::unique_lock<std::mutex> jobsAccess{threadLocks[i]}; //when its this threads turn
            stopped = true;
        }
    }
    

    ~ThreadPool()
    {
        stop();
        delete[] threadLocks;
        delete[] jobListener;
        delete[] jobs;
        delete[] workers;
    }
};
