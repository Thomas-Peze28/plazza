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

namespace Plazza
{
    class Producer
    {
    public:
        Producer();
        Producer(unsigned int id);
        ~Producer() = default;

        void tryProduce();
        void produceRoutine();

    private:
        unsigned int _id;
        std::queue<int> _queue;
        std::mutex _mutex;
    };
}