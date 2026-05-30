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

bool Plazza::Core::trySendOrder(const Order &singlePizza, Logger &logger)
{
    for (auto& kitchen : _kitchens) {
        if (kitchen.currentLoad < 2 * _cooks) {
            std::string mqName = "/plazza_kitchen_" + std::to_string(kitchen.id);
            MessageQueue mq(mqName);
            
            if (mq.sendMessage(singlePizza)) {
                kitchen.currentLoad++;
                logger.log("Pizza envoyée à la cuisine existante " + std::to_string(kitchen.id));
                return true;
            }
        }
    }
    return false;
}

int Plazza::Core::createKitchen(const Order &singlePizza, Logger &logger)
{
    _kitchenCount++;
    std::string mqName = "/plazza_kitchen_" + std::to_string(_kitchenCount);
    
    pid_t pid = fork();

    if (pid == -1) {
        logger.log("Erreur : Impossible de fork.");
        return -1;
    }
    else if (pid == 0) {
        MessageQueue mq(mqName);
        Order receivedPizza;

        logger.log("[Nouvelle Cuisine " + std::to_string(_kitchenCount) + "] Ouverture");
        if (mq.receiveMessage(receivedPizza)) {
            logger.log("[Cuisine " + std::to_string(_kitchenCount) + "] J'ai reçu une pizza du type " + std::to_string(receivedPizza.type));
        }
        mq.closeQueue();
        exit(0);
    } 
    else {
        MessageQueue mq(mqName);
        if (mq.sendMessage(singlePizza)) {
            logger.log("Nouvelle cuisine " + std::to_string(_kitchenCount) + " créée. Pizza envoyée.");
            _kitchens.emplace_back(_kitchenCount, 1, pid);
            return _kitchenCount;
        }
    }
    return -1;
}

void Plazza::Core::handleOrder(const Order &order, Logger &logger, int &kitchenId)
{
    for (int i = 0; i < order.amount; ++i) {
    
        Order singlePizza = order;
        singlePizza.amount = 1; 

        if (!trySendOrder(singlePizza, logger)) {
            int newKitchenId = createKitchen(singlePizza, logger);
            if (newKitchenId != -1) {
                kitchenId = newKitchenId;
            } else {
                logger.log("Erreur : Impossible de créer une nouvelle cuisine.");
            }
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