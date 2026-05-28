/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Parser
*/

#pragma once

    #include <string>
    #include <iostream>

    #include "RTCError.hpp"

namespace Plazza
{
    class Core;

    class Parser
    {
        public:
            Parser() = default;
            ~Parser() = default;

            void parseArguments(int argc, char *argv[], Core &core);
            void parseCommand(const std::string &command);
    };
};