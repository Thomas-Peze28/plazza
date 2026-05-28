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
    #include "IRecipe.hpp"

namespace Plazza
{
    class Core;

    struct Order {
        PizzaType type;
        PizzaSize size;
        int amount;
    };

    class Parser
    {
        public:
            Parser() = default;
            ~Parser() = default;

            void parseArguments(int argc, char *argv[], Core &core);
            std::vector<Order> parseCommand(const std::string &command);
        
        private:
            bool parseOrder(const std::string &orderStr, Order &order);
            bool parseType(const std::string &typeStr, PizzaType &type);
            bool parseSize(const std::string &sizeStr, PizzaSize &size);
            bool parseAmount(const std::string &amountStr, int &amount);
    };
};