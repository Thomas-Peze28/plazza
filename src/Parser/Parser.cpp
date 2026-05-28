/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Parser
*/

#include "Parser.hpp"
#include "Core.hpp"

namespace Plazza
{
    /**
    * Parses the command-line arguments.
    * @param argc The number of command-line arguments.
    * @param argv The array of command-line arguments.
    * @param core The core instance.
    **/
    void Parser::parseArguments(int argc, char *argv[], Core &core)
    {
        (void)argc;
        if (argv[1] != NULL)
            core._multiplier = std::stof(argv[1]);
        if (argv[2] != NULL)
            core._cooks = std::stoi(argv[2]);
        if (argv[3] != NULL)
            core._refillTime = std::stoi(argv[3]);
    }

    /**
    * Parses a command.
    * @param command The command to parse.
    **/
    void Parser::parseCommand(const std::string &command)
    {
        std::cout << "Parsing command: " << command << std::endl;
    }
}