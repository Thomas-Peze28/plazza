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
* Tries to send an order to an existing kitchen.
* @param singlePizza The order to be sent.
* @param logger The logger for logging messages.
* @return True if the order was sent successfully, false otherwise.
**/
bool Plazza::Core::trySendOrder(const Order &singlePizza, Logger &logger)
{
    std::lock_guard<std::mutex> lock(_kitchensMutex);

    for (auto& kitchen : _kitchens) {
        if (kitchen.currentLoad < 2 * _cooks) {
            std::string mqName = "/plazza_kitchen_" + std::to_string(kitchen.id);
            MessageQueue<Order> mq(mqName);
            
            if (mq.sendMessage(singlePizza)) {
                kitchen.currentLoad++;
                logger.log("Pizza sent to existing kitchen " + std::to_string(kitchen.id));
                return true;
            }
        }
    }
    return false;
}

/**
* Creates a new kitchen.
* @param singlePizza The order for the new kitchen.
* @param logger The logger for logging messages.
* @return The ID of the created kitchen, or -1 if creation failed.
**/
int Plazza::Core::createKitchen(const Order &singlePizza, Logger &logger)
{
    _kitchenCount++;
    std::string mqName = "/plazza_kitchen_" + std::to_string(_kitchenCount);
    
    mq_unlink(mqName.c_str());

    pid_t pid = fork();

    if (pid == -1) {
        logger.log("Error : Failed to fork.");
        return -1;
    }
    else if (pid == 0) {
        logger.log("New kitchen " + std::to_string(_kitchenCount) + " created. Waiting for orders...");
        Kitchen kitchen(_kitchenCount, mqName, _cooks, _multiplier, _refillTime);
        kitchen.run();
        exit(0);
    } 
    else {
        MessageQueue<Order> mq(mqName);
        if (mq.sendMessage(singlePizza)) {
            logger.log("New kitchen " + std::to_string(_kitchenCount) + " created. Pizza sent.");
            std::lock_guard<std::mutex> lock(_kitchensMutex);
            _kitchens.emplace_back(_kitchenCount, 1, pid);
            return _kitchenCount;
        }
    }
    return -1;
}

/**
* Handles an order by sending it to an existing kitchen or creating a new one.
* @param order The order to be handled.
* @param logger The logger for logging messages.
**/
void Plazza::Core::handleOrder(const Order &order, Logger &logger)
{
    for (int i = 0; i < order.amount; ++i) {
    
        Order singlePizza = order;
        singlePizza.amount = 1; 

        if (!trySendOrder(singlePizza, logger)) {
            int newKitchenId = createKitchen(singlePizza, logger);
            if (newKitchenId == -1) {
                logger.log("Error: Failed to create a new kitchen for the order.");
            }
        }
    }
}

void Plazza::Core::check_status()
{
    std::lock_guard<std::mutex> lock(_kitchensMutex);
    std::cout << "Current kitchens status:" << std::endl;
    for (const auto& kitchen : _kitchens) {
        std::cout << "Kitchen " << kitchen.id << ": " << kitchen.currentLoad << "/" << 2 * _cooks << " pizzas in progress" << std::endl;
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

    std::thread listener(&Core::listenKitchens, this);
    listener.detach();

    while (!_signal) {
        std::cout << "> ";
        std::string input;

        if (!std::getline(std::cin, input) || input == "exit") {
            break;
        }
        if (input == "clear") {
            system("clear");
            continue;
        }
        if (input == "status") {
            check_status();
            continue;
        }
        if (input.empty())
            continue;
        std::vector<Order> orders = _parser.parseCommand(input);
        for (const auto &order : orders) {
            handleOrder(order, logger);
        }
    }
    logger.log("Closing Plazza...");
    return 0;
}

/**
* Listens for messages from the kitchens to update their load status.
**/
void Plazza::Core::listenKitchens()
{
    Logger logger;

    mq_unlink("/plazza_core_queue");
    MessageQueue<CoreMessage> coreMq("/plazza_core_queue");

    while (!_signal) {
        CoreMessage msgCore;
        if (coreMq.receiveMessage(msgCore)) {
           std::lock_guard<std::mutex> lock(_kitchensMutex);

           for (auto it = _kitchens.begin(); it != _kitchens.end(); ++it) {
               if (it->id == msgCore.kitchenId) {
                   if (msgCore.isClosing) {
                       logger.log("Kitchen " + std::to_string(it->id) + " is closing. Removing from list.");
                       _kitchens.erase(it);
                   } else {
                       it->currentLoad--;
                       logger.log("Kitchen " + std::to_string(it->id) + " finished a pizza. Current load: " + std::to_string(it->currentLoad) + "/" + std::to_string(2 * _cooks));
                   }
                   break;
               }
           }
        }
    }
}