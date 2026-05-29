/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Kitchen
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_
    #include <vector>
    #include <map>
    #include <thread>
    #include <semaphore.h>
    #include "IRecipe.hpp"
    #include "ReginaRecipe.hpp"
    #include "MargaritaRecipe.hpp"
    #include "AmericanaRecipe.hpp"
    #include "FantasiaRecipe.hpp"

namespace Plazza
{
    class Kitchen
    {
    public:
        Kitchen();
        ~Kitchen();

    private:
        std::vector<std::thread> _cooks;
        std::map <Ingredients, int> _stock;
        sem_t _accessStock;
    };
};

#endif /* !KITCHEN_HPP_ */
