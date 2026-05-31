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
    _stock[DOUGH] = 5;
    _stock[TOMATO] = 5;
    _stock[GRUYERE] = 5;
    _stock[HAM] = 5;
    _stock[MUSHROOMS] = 5;
    _stock[STEAK] = 5;
    _stock[EGGPLANT] = 5;
    _stock[GOAT_CHEESE] = 5;
    _stock[CHIEF_LOVE] = 5;

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
            _isOpen = false;
            if (_refillThread.joinable())
                _refillThread.join();
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

    {
        std::lock_guard<std::mutex> lock(_accessStock);
        while (_stock[DOUGH] <= 0 || _stock[TOMATO] <= 0 || _stock[GRUYERE] <= 0 ||
            (pizza.type == Regina && (_stock[HAM] <= 0 || _stock[MUSHROOMS] <= 0)) ||
            (pizza.type == Americana && _stock[STEAK] <= 0) ||
            (pizza.type == Fantasia && (_stock[EGGPLANT] <= 0 || _stock[GOAT_CHEESE] <= 0 || _stock[CHIEF_LOVE] <= 0))) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        _stock[DOUGH] -= 1;
        _stock[TOMATO] -= 1;
        _stock[GRUYERE] -= 1;
        if (pizza.type == Regina) {
            _stock[HAM] -= 1;
            _stock[MUSHROOMS] -= 1;
        } else if (pizza.type == Americana) {
            _stock[STEAK] -= 1;
        } else if (pizza.type == Fantasia) {
            _stock[EGGPLANT] -= 1;
            _stock[GOAT_CHEESE] -= 1;
            _stock[CHIEF_LOVE] -= 1;
        }
    }
    if (pizza.type == Margarita) {
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int64_t>(1 * _multiplier)));
    } else if (pizza.type == Americana || pizza.type == Regina) {
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int64_t>(2 * _multiplier)));
    } else if (pizza.type == Fantasia) {
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int64_t>(4 * _multiplier)));
    }
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