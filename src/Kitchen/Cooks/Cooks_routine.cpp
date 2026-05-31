/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Cooks_routine
*/

#include "Cooks_routine.hpp"
#include <chrono>
#include <thread>

Plazza::Cooks_routine::Cooks_routine()
{
}

Plazza::Cooks_routine::~Cooks_routine()
{
}

void Plazza::Cooks_routine::run()
{
    while (!Core::_signal) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
