/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** main
*/

#include "Core.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[])
{
    try {
        Plazza::Core core;
        Plazza::Parser parser;

        if (core.displayHelp(argc, argv))
            return 0;
        
        parser.parseArguments(argc, argv);

        return core.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
