/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** main
*/

#include "Core.hpp"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    try {
        Plazza::Core core;

        return core.run();
    
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
