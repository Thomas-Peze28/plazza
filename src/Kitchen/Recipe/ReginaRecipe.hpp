/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** ReginaRecipe
*/

#ifndef REGINARECIPE_HPP_
#define REGINARECIPE_HPP_
    #include <vector>
    #include "IRecipe.hpp"

namespace Plazza {
    class ReginaRecipe : public IRecipe {
        public:
            ReginaRecipe();
            ~ReginaRecipe() override = default;

            std::vector<Ingredients> getIngredients() const override;
            int getCookingTime() const override;
    
        private:
            std::vector<Ingredients> _ingredients;
            int _cookingTime;
    };
};

#endif /* !REGINARECIPE_HPP_ */
