#pragma once
#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
#include "atomic_queue/atomic_queue.h"

template <uint_fast8_t threads = 0, size_t queueSize = 512>
class ThreadPool final
{
private:
    bool stopped = false, paused = true;

    atomic_queue::AtomicQueue2<std::function<void()>, queueSize> jobs;
    std::thread *workers; //threads
    uint_fast8_t threadCount;
    std::atomic<size_t> extraSize = 0;

    void Stop()
    {
        stopped = true;
    }

public:
    ThreadPool()
    {
        threadCount = threads ? threads : std::thread::hardware_concurrency();
        workers = new std::thread[threadCount];

        for (uint_fast8_t i = 0; i < threadCount; ++i)
            workers[i] = std::thread([this, i]() {
                std::function<void()> job;
                while (!stopped)
                {
                    while (paused)
                    {
                    }
                    if (jobs.was_size() && !paused)
                    {
                        while (!jobs.try_pop(job))
                        {
                        }
                        job();
                        --extraSize;
                    }
                }
            });
    }

    void AddTask(std::function<void()> func)
    {
        //adding to its job queue
        jobs.push(func);
        ++extraSize;
    }
    size_t Jobs()
    {
        return extraSize;
    }

    void Start()
    {
        paused = false;
    }
    void Pause()
    {
        paused = true;
    }

    ~ThreadPool()
    {
        Stop();
        //closing threads properly
        for (size_t i = 0; i < threadCount; ++i)
            if (workers[i].joinable())
                workers[i].join();
        delete[] workers;
    }
};