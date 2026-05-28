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
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::GRUYERE, Ingridients::HAM, Ingridients::MUSHROOMS};
    _cookingTime = 2;
}
