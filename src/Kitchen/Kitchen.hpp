/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Kitchen
*/

#pragma once

#include <vector>
#include <map>
#include <thread>
#include <semaphore.h>

#include "IRecipe.hpp"
#include "ReginaRecipe.hpp"
#include "MargaritaRecipe.hpp"
#include "AmericanaRecipe.hpp"
#include "FantasiaRecipe.hpp"
#include "MessageQueue.hpp"

namespace Plazza
{
    class Kitchen
    {
    public:
        Kitchen(int id, const std::string& mqName, int cooks, float multiplier, int refillTime);
        ~Kitchen() = default;

        void run();

    private:
        std::vector<std::thread> _cooks;
        std::map <Ingredients, int> _stock;
        sem_t _accessStock;

        int _id;
        std::string _mqName;
        int _cooks;
        float _multiplier;
        int _refillTime;
    };
};