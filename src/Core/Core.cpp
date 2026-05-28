/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Core
*/

#include "Core.hpp"

bool Plazza::Core::_signal = false;

/**
* Displays help information for the plazza.
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
* @return True if help was displayed, false otherwise.
**/
bool Plazza::Core::displayHelp(int argc, char *argv[])
{
    for (unsigned int i = 0; argv[i] != NULL; i++) {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            std::cout << "USAGE: ./plazza <multiplier> <cooks_per_kitchen> <stock_refresh_ms>\n"
                      << "  <multiplier> : float or int (0 < value, can be < 1 to divide cooking time)\n"
                      << "  <cooks_per_kitchen> : integer number of cooks per kitchen\n"
                      << "  <stock_refresh_ms> : integer milliseconds for ingredient replenishment" << std::endl;
            return true;
        }
    }
    if (argc != 4) {
        std::cout << "USAGE: ./plazza <multiplier> <cooks_per_kitchen> <stock_refresh_ms>" << std::endl;
        return true;
    }
    return false;
}

/**
* Handles the signal for interrupting the plazza.
* @param signal The signal received.
**/
void Plazza::Core::signalHandler(int signal)
{
    if (signal == SIGINT) {
        _signal = true;
    }
}

/**
* Runs the plazza.
* @return The result of the plazza execution.
**/
int Plazza::Core::run()
{
    Logger logger;
    logger.log("Plazza Reception is open. Waiting for orders...");

    while (!_signal) {
        std::cout << "> ";
        std::string input;

        if (!std::getline(std::cin, input)) {
            break; 
        }

        if (input == "status") {
            logger.log("Status check requested (To be implemented).");
            continue;
        }
        
        if (input.empty())
            continue;

        std::vector<Order> orders = _parser.parseCommand(input);

        for (const auto& order : orders) {
            std::ostringstream ss;
            ss << "Order Accepted: Type " << order.type << ", Size " << order.size << ", Amount " << order.amount;
            logger.log(ss.str());
        }
    }
    logger.log("Closing Plazza...");
    return 0;
}