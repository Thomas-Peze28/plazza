/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** ThreadPool
*/

#pragma once

#include <queue>
#include <mutex>
#include <iostream>
#include <thread>
#include <functional>

#include "SafeQueue.hpp"

namespace Plazza
{
    class ThreadPool
    {
    public:
        ThreadPool(unsigned nbThreads);
        ~ThreadPool();

        void enqueueTask(std::function<void()> task);
        void stop();

    private:
        std::vector<std::thread> _workers;
        SafeQueue<std::function<void()>> _taskQueue;
        bool _stop;
    };
}