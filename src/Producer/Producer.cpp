/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Producer
*/

#include "Producer.hpp"

Plazza::Producer::Producer(unsigned int id, SafeQueue &queue, bool &signal) 
    : _id(id), _queue(queue), _signal(signal)
{
}

void Plazza::Producer::produceRoutine(std::size_t itemsToProduce)
{
    for (std::size_t j = 0; j < itemsToProduce; ++j) {
        if (_signal)
            break;
            
        int value = rand() % 100;
        _queue.push(value);
        
        std::cout << "Producer " << _id << " produced: " << value << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}