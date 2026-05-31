/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** FantasiaRecipe
*/

#ifndef FANTASIARECIPE_HPP_
#define FANTASIARECIPE_HPP_
    #include <vector>
    #include "IRecipe.hpp"

namespace Plazza {
    class FantasiaRecipe : public IRecipe {
        public:
            FantasiaRecipe();
            ~FantasiaRecipe() override = default;
        private:
            std::vector<Ingredients> _ingredients;
            int _cookingTime;
    };
};

#endif /* !FANTASIARECIPE_HPP_ */
