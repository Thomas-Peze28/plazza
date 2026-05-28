/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Recipe
*/

#include "Recipe.hpp"

// - Margarita: Contains dough, tomato and gruyere. Baked in 1 sec * multiplier.
Plazza::MargaritaRecipe::MargaritaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::GRUYERE};
    _cookingTime = 1;
}

// - Regina: Contains dough, tomato, gruyere, ham, mushrooms. Baked in 2 secs * multiplier.
Plazza::ReginaRecipe::ReginaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::GRUYERE, Ingridients::HAM, Ingridients::MUSHROOMS};
    _cookingTime = 2;
}

// - Americana: Contains dough, tomato, gruyere, steak. Baked in 2 secs * multiplier.
Plazza::AmericanaRecipe::AmericanaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::GRUYERE, Ingridients::STEAK};
    _cookingTime = 2;
}

// - Fantasia: Contains dough, tomato, eggplant, goat cheese and chief love. Baked in 4 secs * multiplier
Plazza::FantasiaRecipe::FantasiaRecipe()
{
    _ingridients = {Ingridients::DOUGH, Ingridients::TOMATO, Ingridients::EGGPLANT, Ingridients::GOAT_CHEESE, Ingridients::CHIEF_LOVE};
    _cookingTime = 4;
}
