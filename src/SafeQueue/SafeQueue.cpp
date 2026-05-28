/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** SafeQueue
*/

#include "SafeQueue.hpp"

/**
* @brief Construct a new Safe Queue
**/
Plazza::SafeQueue::SafeQueue() : _mutex()
{
}

/**
* @brief Push a value onto the queue
* @param value The value to push
**/
void Plazza::SafeQueue::push(int value)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _queue.push(value);
}

/**
* @brief Try to pop a value from the queue
* @param value Variable to store the popped value
* @return true if a value was popped, false if the queue was empty
**/
bool Plazza::SafeQueue::tryPop(int &value)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_queue.empty())
        return false;
    value = _queue.front();
    _queue.pop();
    return true;
}
