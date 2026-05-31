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

/**
* Check if the kitchen has the required ingredients for a recipe.
* @param recipeIngredients The list of required ingredients.
* @return True if all ingredients are available, false otherwise.
**/
bool Plazza::Kitchen::hasIngredients(const std::vector<Ingredients> &recipeIngredients)
{
    std::lock_guard<std::mutex> lock(_accessStock);
    std::map<Ingredients, int> requiredIngredients;

    for (const auto& ingredient : recipeIngredients) {
        requiredIngredients[ingredient]++;
    }
    for (const auto& item : requiredIngredients) {
        if (_stock[item.first] < item.second) {
            return false;
        }
    }
    return true;
}

/**
* Consume the required ingredients for a recipe.
* @param recipeIngredients The list of required ingredients.
**/
void Plazza::Kitchen::consumeIngredients(const std::vector<Ingredients> &recipeIngredients)
{
    std::lock_guard<std::mutex> lock(_accessStock);
    std::map<Ingredients, int> requiredIngredients;

    for (const auto& ingredient : recipeIngredients) {
        requiredIngredients[ingredient]++;
    }
    for (const auto& item : requiredIngredients) {
        _stock[item.first] -= item.second;
    }
}

void Plazza::Kitchen::cookPizza(Order pizza)
{
    Logger logger;
    std::unique_ptr<IRecipe> recipe;

    switch (pizza.type) {
        case Margarita:
            recipe = std::make_unique<MargaritaRecipe>();
            break;
        case Regina:
            recipe = std::make_unique<ReginaRecipe>();
            break;
        case Americana:
            recipe = std::make_unique<AmericanaRecipe>();
            break;
        case Fantasia:
            recipe = std::make_unique<FantasiaRecipe>();
            break;
        default:
            return; 
    }

    if (recipe) {

        auto recipeIngredients = recipe->getIngredients();

        while (!hasIngredients(recipeIngredients)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        consumeIngredients(recipeIngredients);
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(recipe->getCookingTime() * _multiplier)));

        logger.log("Kitchen " + std::to_string(_id) + " finished pizza type " + std::to_string(pizza.type));

        MessageQueue<CoreMessage> coreMq("/plazza_core_queue");
        CoreMessage msgCore;
        msgCore.kitchenId = _id;
        msgCore.isClosing = false;
        coreMq.sendMessage(msgCore);
    }
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