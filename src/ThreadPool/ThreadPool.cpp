/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** ThreadPool
*/

#include "ThreadPool.hpp"

Plazza::ThreadPool::ThreadPool(unsigned nbThreads) : _stop(false)
{
    for (unsigned i = 0; i < nbThreads; ++i) {
        _workers.emplace_back([this] {
            while (true) {
                std::function<void()> task = _taskQueue.pop();
                if (!task && _stop)
                    return;
                if (task)
                    task();
            }
        });
    }
}

Plazza::ThreadPool::~ThreadPool()
{
    stop();
}

void Plazza::ThreadPool::enqueueTask(std::function<void()> task)
{
    _taskQueue.push(task);
}

void Plazza::ThreadPool::stop()
{
    _stop = true;
    _taskQueue.stop();
    for (std::thread &worker : _workers) {
        if (worker.joinable())
            worker.join();
    }
}