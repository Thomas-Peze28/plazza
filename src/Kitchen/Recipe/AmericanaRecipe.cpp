/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** AmericanaRecipe
*/

#include "AmericanaRecipe.hpp"

// - Americana: Contains dough, tomato, gruyere, steak. Baked in 2 secs * multiplier.
Plazza::AmericanaRecipe::AmericanaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::GRUYERE, Ingridients::STEAK};
    _cookingTime = 2;
}