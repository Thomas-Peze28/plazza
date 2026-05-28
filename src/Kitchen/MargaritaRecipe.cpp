/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** MargaritaRecipe
*/

#include "MargaritaRecipe.hpp"

// - Margarita: Contains dough, tomato and gruyere. Baked in 1 sec * multiplier.
Plazza::MargaritaRecipe::MargaritaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::GRUYERE};
    _cookingTime = 1;
}
