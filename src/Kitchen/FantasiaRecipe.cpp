/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** FantasiaRecipe
*/

#include "FantasiaRecipe.hpp"

// - Fantasia: Contains dough, tomato, eggplant, goat cheese and chief love. Baked in 4 secs * multiplier
Plazza::FantasiaRecipe::FantasiaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::EGGPLANT, Ingridients::GOAT_CHEESE, Ingridients::CHIEF_LOVE};
    _cookingTime = 4;
}
