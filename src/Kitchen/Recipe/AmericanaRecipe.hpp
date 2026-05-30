/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** AmericanaRecipe
*/

#ifndef AMERICANARECIPE_HPP_
#define AMERICANARECIPE_HPP_
    #include <vector>
    #include "IRecipe.hpp"

namespace Plazza {
    class AmericanaRecipe : public IRecipe {
        public:
            AmericanaRecipe();
            ~AmericanaRecipe() override = default;
        private:
            std::vector<Ingredients> _ingredients;
            int _cookingTime;
    };
};

#endif /* !AMERICANARECIPE_HPP_ */
