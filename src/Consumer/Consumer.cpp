/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Consumer
*/

#include "Consumer.hpp"

Plazza::Consumer::Consumer() : _id(0), _queue(), _mutex()
{
}

Plazza::Consumer::Consumer(unsigned int id) : _id(id), _queue(), _mutex()
{
}

void Plazza::Consumer::tryConsume()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (!_queue.empty()) {
        int value = _queue.front();
        _queue.pop();
        std::cout << "Consumed: " << value << std::endl;
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Plazza::Consumer::consumeRoutine()
{
    while (true) {
        tryConsume();
    }
}