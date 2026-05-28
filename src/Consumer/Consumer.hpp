/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Consumer
*/

#pragma once

#include <queue>
#include <mutex>
#include <iostream>
#include <thread>

#include "SafeQueue.hpp"

namespace Plazza
{
    class Consumer
    {
    public:
        Consumer(unsigned int id, SafeQueue<int> &queue, bool &signal);
        ~Consumer() = default;

        void consumeRoutine();

    private:
        unsigned int _id;
        SafeQueue<int> &_queue;
        bool &_signal;
    };
}