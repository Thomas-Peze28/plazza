/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Recipe
*/

#ifndef RECIPE_HPP_
#define RECIPE_HPP_
    #include <vector>

namespace Plazza {
    enum PizzaType
    {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8
    };

    enum PizzaSize
    {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    enum Ingridients
    {
        DOUGH,
        TOMATO,
        GRUYERE,
        HAM,
        MUSHROOMS,
        STEAK,
        EGGPLANT,
        GOAT_CHEESE,
        CHIEF_LOVE
    };

    class IRecipe {
        public:
            virtual ~IRecipe() = default;
            virtual std::vector<Ingridients> getIngridients() const = 0;
            virtual int getCookingTime() const = 0;
    };

    class MargaritaRecipe : public IRecipe {
        public:
            MargaritaRecipe();
            ~MargaritaRecipe() override = default;
        private:
            std::vector<Ingridients> _ingridients;
            int _cookingTime;
    };

    class ReginaRecipe : public IRecipe {
        public:
            ReginaRecipe();
            ~ReginaRecipe() override = default;
        private:
            std::vector<Ingridients> _ingridients;
            int _cookingTime;
    };

    class AmericanaRecipe : public IRecipe {
        public:
            AmericanaRecipe();
            ~AmericanaRecipe() override = default;
        private:
            std::vector<Ingridients> _ingridients;
            int _cookingTime;
    };

    class FantasiaRecipe : public IRecipe {
        public:
            FantasiaRecipe();
            ~FantasiaRecipe() override = default;
        private:
            std::vector<Ingridients> _ingridients;
            int _cookingTime;
    };
};

#endif /* !RECIPE_HPP_ */
