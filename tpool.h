#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>       
#include <functional>

template <uint_fast8_t threadCount>
class ThreadPool
{
private:
    bool stopped = false, paused = true;

    std::mutex threadLocks[threadCount];
    std::condition_variable jobListener[threadCount];
    std::queue<std::function<void()> > jobs[threadCount]; //a queue for each thread
    std::thread workers[threadCount];
    uint_fast8_t nextIndex = 0;
public:
    ThreadPool()
    {
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
                                jobs[i].pop();
                                job();
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
            //std::cout<<"job added\n";
        }
        nextIndex = nextIndex==(threadCount-1)?0:nextIndex+1;
    }
    size_t JobsLeft() //broken
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
    }
};
