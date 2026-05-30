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
    
        void handleOrder(const Order &order, Logger &logger, int &kitchenId);
    };
};
