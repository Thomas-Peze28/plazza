/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** MargaritaRecipe
*/

#ifndef MARGARITARECIPE_HPP_
#define MARGARITARECIPE_HPP_
    #include <vector>
    #include "IRecipe.hpp"

namespace Plazza {
    class MargaritaRecipe : public IRecipe {
        public:
            MargaritaRecipe();
            ~MargaritaRecipe() override = default;
        private:
            std::vector<Ingredients> _ingredients;
            int _cookingTime;
    };
};

#endif /* !MARGARITARECIPE_HPP_ */
