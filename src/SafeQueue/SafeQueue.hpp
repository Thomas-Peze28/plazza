/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** SafeQueue
*/

#pragma once

#include "ISafeQueue.hpp"
#include <queue>
#include <mutex>

namespace Plazza
{
    class SafeQueue : public ISafeQueue
    {
    public:
        SafeQueue();
        ~SafeQueue() override = default;

        void push(int value) override;
        bool tryPop(int &value) override;

    private:
        std::queue<int> _queue;
        std::mutex _mutex;
    };
}