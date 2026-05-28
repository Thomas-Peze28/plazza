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
        private:
            std::vector<Ingridients> _ingridients;
            int _cookingTime;
    };
};

#endif /* !REGINARECIPE_HPP_ */
