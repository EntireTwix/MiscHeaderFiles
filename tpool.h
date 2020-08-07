#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <condition_variable>

template <uint_fast8_t threadsArgument = 0>
class ThreadPool
{
private:
    std::thread* workers;
    uint_fast8_t threads = threadsArgument > 0 ? threadsArgument : std::thread::hardware_concurrency();

    //jobLock scope
    std::queue<std::function<void()> > tasks;
    std::mutex jobLock, playingLock;

    size_t taskCount = 0;

    bool playing = false; //by default paused
    bool stopped = false;

    std::condition_variable jobVar, pausedVar;

public:
    ThreadPool()
    {
        workers = new std::thread[threads];
        for (uint_fast8_t i = 0; i < threads; ++i)
        {
            workers[i] = std::thread([this, i]() {
                size_t tCount = 0;
                std::function<void()> job;

                while (1)
                {
                    //std::cout<<"1\n";

                    if (!stopped) //is thread running
                    {
                        //std::cout<<"2\n";
                        //std::cout<<std::to_string(tCount)+" : tCount\n";

                        if (tasks.size()) //there are tasks that arent being worked on
                        {
                            //setting up job
                            jobLock.lock();
                            job = tasks.front();
                            tasks.pop();
                            jobLock.unlock();
                            //std::cout<<"3\n";

                        }
                        else
                        {
                            {
                                //std::cout<<"4\n";
                                std::unique_lock<std::mutex> jobL{ jobLock };
                                jobVar.wait(jobL); //waiting for new job or stopped notification
                            }

                            //std::cout<<"5\n";

                            if (stopped)
                            {
                                //std::cout<<"6\n";
                                break;
                            }
                            //std::cout<<"7\n";

                            job = tasks.front();
                            tasks.pop();

                        }

                        //std::cout<<"8\n";

                        if (playing)
                        {
                            //std::cout<<"working\n";
                            job();
                            //std::cout<<"done working\n";
                            jobLock.lock();
                            --taskCount;
                            jobLock.unlock();
                            //std::cout<<"9\n";

                        }
                        else //if paused
                        {
                            {
                                std::unique_lock<std::mutex> pausedL{ playingLock };
                                pausedVar.wait(pausedL); //waiting for unpaused or stopped notification
                            }
                            //std::cout<<"10\n";

                            if (stopped)
                            {
                                //std::cout<<"11\n";

                                break;

                            }
                            else
                            {
                                //std::cout<<"working\n";
                                job();
                                //std::cout<<"done working\n";
                                jobLock.lock();
                                --taskCount;
                                jobLock.unlock();
                                //std::cout<<"12\n";

                            }
                        }

                    }
                    else
                    {
                        //std::cout<<"13\n";

                        break;
                    }
                }
                //std::cout<<"thread died\n";
                });
        }

    }
    void Pause()
    {
        playing = false;
    }
    void Start()
    {
        playing = true;

        //std::cout<<"all threads unpaused\n";
        pausedVar.notify_all(); //break all waits based on pause

    }
    size_t Jobs()
    {
        std::unique_lock<std::mutex> taskL{ jobLock };
        return taskCount;
    }
    void AddTask(std::function<void()> func)
    {
        jobLock.lock();
        tasks.push(func);
        ++taskCount;
        jobLock.unlock();


        jobVar.notify_one(); //let one of the threads know of the new job
        //std::cout<<"one thread notified of new job\n";
    }


    ~ThreadPool()
    {
        //std::cout<<"deconstruction started\n";
        stopped = true;

        playing = true;

        pausedVar.notify_all();
        jobVar.notify_all();

        for (uint_fast8_t i = 0; i < threads; ++i)
        {
            if (workers[i].joinable()) workers[i].join();
        }
        delete[] workers;
    }
};
