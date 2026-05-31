/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** FantasiaRecipe
*/

#include "FantasiaRecipe.hpp"

/**
* @brief Construct a new FantasiaRecipe object
* @details - Fantasia: Contains dough, tomato, eggplant, goat cheese and chief love. Baked in 4 secs * multiplier.
**/
Plazza::FantasiaRecipe::FantasiaRecipe()
{
    _ingredients = {Plazza::Ingredients::DOUGH, Plazza::Ingredients::TOMATO, Plazza::Ingredients::EGGPLANT, Plazza::Ingredients::GOAT_CHEESE, Plazza::Ingredients::CHIEF_LOVE};
    _cookingTime = 4;
}

std::vector<Plazza::Ingredients> Plazza::FantasiaRecipe::getIngredients() const
{
    return _ingredients;
}

int Plazza::FantasiaRecipe::getCookingTime() const
{
    return _cookingTime;
}
