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
    for (unsigned int i = 0; argv[i] != NULL; i++)
    {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
        {
            std::cout << "USAGE: ./plazza <multiplier> <cooks_per_kitchen> <stock_refresh_ms>\n"
                      << "  <multiplier> : float or int (0 < value, can be < 1 to divide cooking time)\n"
                      << "  <cooks_per_kitchen> : integer number of cooks per kitchen\n"
                      << "  <stock_refresh_ms> : integer milliseconds for ingredient replenishment" << std::endl;
            return true;
        }
    }
    if (argc != 4)
    {
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
    if (signal == SIGINT)
    {
        _signal = true;
    }
}

void Plazza::Core::handleOrder(const Order &order, Logger &logger, int &kitchenId)
{
    for (int i = 0; i < order.amount; ++i)
    {
        Order singlePizza = order;
        singlePizza.amount = 1;

        std::string mqName = "/plazza_kitchen_" + std::to_string(kitchenId);

        pid_t pid = fork();

        if (pid == -1) {
            ErrorMajor("Failed to fork process for kitchen.");
            continue;
        }
        else if (pid == 0) {
            MessageQueue mq(mqName);
            Order receivedPizza;

            logger.log("[Cuisine " + std::to_string(kitchenId) + "]: ouvertur.");
            if (mq.receiveMessage(receivedPizza)) {
                logger.log("[Cuisine " + std::to_string(kitchenId) + "]: j'ai reçu la pizza du type " + std::to_string(receivedPizza.type) + ".");
            }
            mq.closeQueue();
            logger.log("[Cuisine " + std::to_string(kitchenId) + "]: fermeture.");
            exit(0);
        }
        else {
            MessageQueue mq(mqName);

            if (mq.sendMessage(singlePizza))
                logger.log("✅ [Réception] Pizza envoyée à la cuisine " + std::to_string(kitchenId));
            kitchenId++;
        }
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
    int kitchenId = 0;

    while (!_signal) {
        std::cout << "> ";
        std::string input;

        if (!std::getline(std::cin, input)) {
            break;
        }

        if (input == "status") {
            logger.log("Status command.");
            continue;
        }

        if (input.empty())
            continue;

        std::vector<Order> orders = _parser.parseCommand(input);

        for (const auto &order : orders) {
            handleOrder(order, logger, kitchenId);
        }
    }
    logger.log("Closing Plazza...");
    return 0;
}