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
#include <condition_variable>

namespace Plazza
{
    template <typename Type>
    class SafeQueue : public ISafeQueue<Type>
    {
    public:
        SafeQueue() = default;
        ~SafeQueue() override = default;

        /**
        * @brief Push a value onto the queue
        * @param value The value to push
        **/
        void push(Type value) override
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _queue.push(value);
            }
            _condition.notify_one();
        }

        /**
        * @brief Try to pop a value from the queue
        * @param value Variable to store the popped value
        * @return true if a value was popped, false if the queue was empty
        **/
        bool tryPop(Type &value) override
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if (_queue.empty())
                return false;
            value = _queue.front();
            _queue.pop();
            return true;
        }

        /**
        * @brief Pop a value from the queue, blocking if the queue is empty
        * @return The popped value
        **/
        Type pop() override
        {
            std::unique_lock<std::mutex> lock(_mutex);
            
            _condition.wait(lock, [this]() {
                return !_queue.empty() || _stopped;
            });

            if (_queue.empty())
                return Type(); 

            Type value = _queue.front();
            _queue.pop();
            return value;
        }
    
        /**
        * @brief Stop the queue
        **/
        void stop() override
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _stopped = true;
            _condition.notify_all();
        }

    private:
        std::queue<Type> _queue;
        std::mutex _mutex;
        std::condition_variable _condition;
        bool _stopped = false;
    };
}