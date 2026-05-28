/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Consumer
*/

#include "Consumer.hpp"

Plazza::Consumer::Consumer(unsigned int id, SafeQueue &queue, bool &signal) 
    : _id(id), _queue(queue), _signal(signal)
{
}

void Plazza::Consumer::consumeRoutine()
{
    while (!_signal) {
        int value = _queue.pop();
        
        if (!_signal) {
            std::cout << "Consumer " << _id << " consumed: " << value << std::endl;
        }
    }
}