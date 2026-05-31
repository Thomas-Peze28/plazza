/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** MargaritaRecipe
*/

#include "MargaritaRecipe.hpp"

/**
* @brief Construct a new MargaritaRecipe object
* @details - Margarita: Contains dough, tomato and gruyere. Baked in 1 sec * multiplier.
**/
Plazza::MargaritaRecipe::MargaritaRecipe()
{
    _ingredients = {Plazza::Ingredients::DOUGH, Plazza::Ingredients::TOMATO, Plazza::Ingredients::GRUYERE};
    _cookingTime = 1;
}

std::vector<Plazza::Ingredients> Plazza::MargaritaRecipe::getIngredients() const
{
    return _ingredients;
}

int Plazza::MargaritaRecipe::getCookingTime() const
{
    return _cookingTime;
}
