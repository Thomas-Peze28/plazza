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

namespace Plazza
{
    class Consumer
    {
    public:
        Consumer();
        Consumer(unsigned int id);
        ~Consumer() = default;

        void tryConsume();
        void consumeRoutine();

    private:
        unsigned int _id;
        std::queue<int> _queue;
        std::mutex _mutex;
    };
}