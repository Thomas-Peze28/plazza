/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

Plazza::Kitchen::Kitchen(int id, const std::string& mqName, int cooks, float multiplier, int refillTime)
    : _id(id), _mqName(mqName), _cooks(cooks), _multiplier(multiplier), _refillTime(refillTime), _cookPool(cooks)
{
    _stock[Ingredients::DOUGH] = 5;
    _stock[Ingredients::TOMATO] = 5;
    _stock[Ingredients::GRUYERE] = 5;
    _stock[Ingredients::HAM] = 5;
    _stock[Ingredients::MUSHROOMS] = 5;
    _stock[Ingredients::STEAK] = 5;
    _stock[Ingredients::EGGPLANT] = 5;
    _stock[Ingredients::GOAT_CHEESE] = 5;
    _stock[Ingredients::CHIEF_LOVE] = 5;

    _isOpen = true;
    _refillThread = std::thread(&Kitchen::refillStock, this);
}

/**
* Run the kitchen.
**/
void Plazza::Kitchen::run()
{
    Logger logger;
    MessageQueue<Order> mq(_mqName);

    logger.log("Kitchen " + std::to_string(_id) + " is running.");

    while (true) {
        Order receivedPizza;
    
        if (mq.receiveMessageWithTimeout(receivedPizza, 5) && receivedPizza.amount > 0) {
            logger.log("Kitchen " + std::to_string(_id) + " received an order for a pizza of type " + std::to_string(receivedPizza.type));
            
            _cookPool.enqueueTask([this, receivedPizza]() {
                this->cookPizza(receivedPizza);
            });
            
        } else {
            logger.log("Kitchen " + std::to_string(_id) + " is inactive. Closing...");
            MessageQueue<CoreMessage> coreMq("/plazza_core_queue");
            CoreMessage msgCore;
            msgCore.kitchenId = _id;
            msgCore.isClosing = true;
            coreMq.sendMessage(msgCore);
            break;
        }
    }
}

void Plazza::Kitchen::cookPizza(Order pizza)
{
    Logger logger;
        
    std::this_thread::sleep_for(std::chrono::seconds(3));

    logger.log("Kitchen " + std::to_string(_id) + " finished pizza type " + std::to_string(pizza.type));

    MessageQueue<CoreMessage> coreMq("/plazza_core_queue");
    CoreMessage msgCore;
    msgCore.kitchenId = _id;
    msgCore.isClosing = false;
    coreMq.sendMessage(msgCore);
}

/**
* Refill the stock every _refillTime milliseconds.
**/
void Plazza::Kitchen::refillStock()
{
    while (_isOpen) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_refillTime));
        
        std::lock_guard<std::mutex> lock(_accessStock);
        for (auto& item : _stock) {
            item.second += 1;
        }
    }
}