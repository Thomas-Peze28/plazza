/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Core
*/

#pragma once

#include <cstdlib>
#include <string>
#include <iostream>
#include <csignal>
#include <sstream>

#include "RTCError.hpp"
#include "Parser.hpp"
#include "SafeQueue.hpp"
#include "ThreadPool.hpp"
#include "Logger.hpp"
#include "MessageQueue.hpp"

namespace Plazza
{
    /**
    * Represents information about a kitchen.
    * @param id The unique identifier of the kitchen.
    * @param currentLoad The current load of the kitchen.
    * @param pid The process ID of the kitchen.
    **/
    struct KitchenInfo
    {
        int id;
        int currentLoad;
        pid_t pid;
    };

    class Core
    {
    public:
        Core() = default;
        ~Core() = default;

        int run();

        bool displayHelp(int argc, char *argv[]);

        static void signalHandler(int signal);
        static bool _signal;

        float _multiplier;
        int _cooks;
        int _refillTime;

    private:
        Parser _parser;
        std::vector<KitchenInfo> _kitchens;
        unsigned int _kitchenCount = 0;
        std::mutex _kitchensMutex;
    
        void handleOrder(const Order &order, Logger &logger);
        bool trySendOrder(const Order &singlePizza, Logger &logger);
        int createKitchen(const Order &singlePizza, Logger &logger);
        void listenKitchens();
    };
};
