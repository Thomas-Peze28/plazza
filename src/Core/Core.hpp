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

    #include "RTCError.hpp"
    #include "Parser.hpp"
    #include "SafeQueue.hpp"
    #include "Producer.hpp"
    #include "Consumer.hpp"
    

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

        private:
            Parser _parser;
            std::vector<std::thread> _producerThreads;
            std::vector<std::thread> _consumerThreads;
            SafeQueue _queue;
    };
};

