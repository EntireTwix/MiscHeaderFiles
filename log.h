#pragma once
#include <string>
#include "MiscHeaderFiles-master/point.h"
#include "atomic_queue.h"

#define ts std::to_string

struct LogType
{
    const std::string label;
    const RGB color;
};

namespace LogTypes
{
    const LogType ERR{"Error", {255, 0, 0}};
    const LogType INF{"Info", {0, 255, 0}};
    const LogType DBG{"Debug", {0, 0, 255}};
    const LogType EMR{"EMERGENCY!", {255, 0, 0}};
}; // namespace LogTypes

//thread safe
template <size_t max_size = 512>
class Logger
{
private:
    atomic_queue::AtomicQueue2<std::string, max_size> log;

public:
    template <typename... Params>
    void Log(const LogType &LogLabel, Params &&... args)
    {
        log.push('[' + LogLabel.label + "] " + (args + ...) + '\n');
    }
    bool Empty() const
    {
        return log.was_empty();
    }
    std::string Retrieve()
    {
        return log.pop();
    }
};
