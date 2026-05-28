/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Logger
*/

#include "Logger.hpp"
#include <syncstream>

void Plazza::Logger::log(const std::string &message)
{
    std::osyncstream synced_out(std::cout);
    synced_out << message << std::endl;
}
