/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Logger
*/

#pragma once

#include <iostream>
#include <string>

namespace Plazza
{
    class Logger
    {
    public:
        Logger() = default;
        ~Logger() = default;

        void log(const std::string &message);

    private:
    };
}