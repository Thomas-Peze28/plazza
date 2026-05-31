/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

Plazza::Kitchen::Kitchen(int id, const std::string& mqName, int cooks, float multiplier, int refillTime)
    : _id(id), _mqName(mqName), _cooks(cooks), _multiplier(multiplier), _refillTime(refillTime)
{
}

