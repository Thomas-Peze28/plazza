/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

Plazza::Kitchen::Kitchen(const Core &core) : _core(core), _cookPool(core._cooks)
{
    _clock = 0;
    sem_init(&_accessStock, 0, 1);
    _stock[DOUGH] = 5;
    _stock[TOMATO] = 5;
    _stock[GRUYERE] = 5;
    _stock[HAM] = 5;
    _stock[MUSHROOMS] = 5;
    _stock[STEAK] = 5;
    _stock[EGGPLANT] = 5;
    _stock[GOAT_CHEESE] = 5;
    _stock[CHIEF_LOVE] = 5;
}

Plazza::Kitchen::~Kitchen()
{
    sem_destroy(&_accessStock);
}

void Plazza::Kitchen::refillStock()
{
    if (_clock >= _core._refillTime) {
        sem_wait(&_accessStock);
        for (auto &ingredient : _stock) {
            ingredient.second += 1;
        }
        sem_post(&_accessStock);
        _clock = 0;
    }
}

void Plazza::Kitchen::enqueueCookTask(std::function<void()> task)
{
    _cookPool.enqueueTask(std::move(task));
}

// void Plazza::Kitchen::startCooks()
// {
//     // ThreadPool starts its worker threads on construction.
// }
