/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Parser
*/

#include "Parser.hpp"
#include "Core.hpp"
#include <map>

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
    * @return A vector of orders parsed from the command.
    **/
    std::vector<Order> Parser::parseCommand(const std::string &command)
    {
        std::vector<Order> orders;
        std::stringstream ss(command);
        std::string orderStr;

        while (std::getline(ss, orderStr, ';'))
        {
            Order singleOrder;
            if (parseOrder(orderStr, singleOrder))
                orders.push_back(singleOrder);
            else
                ErrorMinor error("Invalid order: " + orderStr);
        }
        return orders;
    }

    /**
    * Parses the type of pizza from a string.
    * @param orderStr The order string to parse.
    * @param order The Order struct to fill with parsed data.
    * @return True if the order was successfully parsed, false otherwise.
    **/
    bool Parser::parseType(const std::string &typeStr, PizzaType &type)
    {
        if (typeStr == "regina")
            type = Regina;
        else if (typeStr == "margarita")
            type = Margarita;
        else if (typeStr == "americana")
            type = Americana;
        else if (typeStr == "fantasia")
            type = Fantasia;
        else
            return false;
        return true;
    }

    /**
    * Parses the size of pizza from a string.
    * @param sizeStr The size string to parse.
    * @param size The PizzaSize enum to fill with parsed data.
    * @return True if the size was successfully parsed.
    **/
    bool Parser::parseSize(const std::string &sizeStr, PizzaSize &size)
    {
        if (sizeStr == "S")
            size = S;
        else if (sizeStr == "M")
            size = M;
        else if (sizeStr == "L")
            size = L;
        else if (sizeStr == "XL")
            size = XL;
        else if (sizeStr == "XXL")
            size = XXL;
        else
            return false;
        return true;
    }
    
    /**
    * Parses the amount of pizza from a string.
    * @param amountStr The amount string to parse.
    * @param amount The integer to fill with parsed data.
    * @return True if the amount was successfully parsed.
    **/
    bool Parser::parseAmount(const std::string &amountStr, int &amount)
    {
        if (amountStr.empty() || amountStr[0] != 'x')
            return false;
        try {
            amount = std::stoi(amountStr.substr(1));
            return amount > 0;
        } catch (...) {
            return false;
        }
    }

    /**
    * Parses a single order string into an Order struct.
    * @param orderStr The order string to parse.
    * @param order The Order struct to fill with parsed data.
    * @return True if the order was successfully parsed, false otherwise.
    **/
    bool Parser::parseOrder(const std::string &orderStr, Order &order)
    {
        std::stringstream ss(orderStr);
        std::string typeStr;
        std::string sizeStr;
        std::string amountStr;

        if (!(ss >> typeStr >> sizeStr >> amountStr))
            return false;

        std::string extra;
        if (ss >> extra)
            return false;

        if (!parseType(typeStr, order.type))
            return false;   
        if (!parseSize(sizeStr, order.size))
            return false;
        if (!parseAmount(amountStr, order.amount))
            return false;
        return true;
    }
};
    