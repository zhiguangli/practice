#pragma once

#include "taskflow.hpp"

namespace tf
{
    struct Executor
    {
        void run(const Taskflow& taskflow)
        {
            auto taskList = taskflow.sort();
            for(const auto& task : taskList)
            {
                task();
            }
        }
    };
}
