/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** AmericanaRecipe
*/

#include "AmericanaRecipe.hpp"

/**
* @brief Construct a new AmericanaRecipe object
* @details - Americana: Contains dough, tomato, gruyere and steak. Baked in 2 secs * multiplier.
**/
Plazza::AmericanaRecipe::AmericanaRecipe()
{
    _ingredients = {Ingredients::DOUGH, Ingredients::TOMATO, Ingredients::GRUYERE, Ingredients::STEAK};
    _cookingTime = 2;
}

std::vector<Ingredients> Plazza::AmericanaRecipe::getIngredients() const
{
    return _ingredients;
}

int Plazza::AmericanaRecipe::getCookingTime() const
{
    return _cookingTime;
}