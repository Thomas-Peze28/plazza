/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Producer
*/

#include "Producer.hpp"

Plazza::Producer::Producer() : _queue(), _mutex()
{
}

Plazza::Producer::Producer(unsigned int id) : _id(id), _queue(), _mutex()
{
}

void Plazza::Producer::tryProduce()
{
    std::lock_guard<std::mutex> lock(_mutex);

    int value = rand() % 100;
    _queue.push(value);
    std::cout << "Produced: " << value << std::endl;
}

void Plazza::Producer::produceRoutine()
{
    while (true) {
        tryProduce();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}