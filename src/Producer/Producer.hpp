/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Producer
*/

#pragma once

#include <queue>
#include <mutex>
#include <iostream>
#include <thread>

#include "SafeQueue.hpp"

namespace Plazza
{
    class Producer
    {
    public:
        Producer(unsigned int id, SafeQueue<int> &queue, bool &signal);
        ~Producer() = default;

        void produceRoutine(std::size_t itemsToProduce);

    private:
        unsigned int _id;
        SafeQueue<int> &_queue;
        bool &_signal;
    };
}