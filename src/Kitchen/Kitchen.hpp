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
#include "Logger.hpp"
#include "MessageQueue.hpp"
#include "ThreadPool.hpp"
#include "Core.hpp"

namespace Plazza
{
    class Kitchen
    {
    public:
        Kitchen(int id, const std::string& mqName, int cooks, float multiplier, int refillTime);
        ~Kitchen() = default;

        void run();

    private:
        std::map <Ingredients, int> _stock;
        std::mutex _accessStock;
        int _id;
        std::string _mqName;
        int _cooks;
        float _multiplier;
        int _refillTime;
        ThreadPool _cookPool;
        std::thread _refillThread;
        bool _isOpen;

        void cookPizza(Order pizza);
        bool hasIngredients(const std::vector<Ingredients> &recipeIngredients);
        void consumeIngredients(const std::vector<Ingredients> &recipeIngredients);
        void refillStock();
        void printStatus();
    };
};