/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** ReginaRecipe
*/

#include "ReginaRecipe.hpp"

/**
* @brief Construct a new ReginaRecipe object
* @details - Regina: Contains dough, tomato, gruyere, ham, mushrooms. Baked in 2 secs * multiplier.
**/
Plazza::ReginaRecipe::ReginaRecipe()
{
    _ingredients = {Ingredients::DOUGH, Ingredients::TOMATO, Ingredients::GRUYERE, Ingredients::HAM, Ingredients::MUSHROOMS};
    _cookingTime = 2;
}

std::vector<Ingredients> Plazza::ReginaRecipe::getIngredients() const
{
    return _ingredients;
}

int Plazza::ReginaRecipe::getCookingTime() const
{
    return _cookingTime;
}