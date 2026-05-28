/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Parser
*/

#include "Parser.hpp"

namespace Plazza
{
    /**
    * Parses the command-line arguments.
    * @param argc The number of command-line arguments.
    * @param argv The array of command-line arguments.
    **/
    void Parser::parseArguments(int argc, char *argv[])
    {
        (void)argc;
        (void)argv;
        std::cout << "Parsing arguments..." << std::endl;
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