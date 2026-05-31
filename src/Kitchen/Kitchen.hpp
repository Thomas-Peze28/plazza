/*
** EPITECH PROJECT, 2026
** G-CCP-400-LIL-4-1-theplazza-23
** File description:
** Kitchen
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_
    #include <functional>
    #include <map>
    #include <semaphore.h>
    #include "IRecipe.hpp"
    #include "ReginaRecipe.hpp"
    #include "MargaritaRecipe.hpp"
    #include "AmericanaRecipe.hpp"
    #include "FantasiaRecipe.hpp"
    #include "Core.hpp"
    #include "ThreadPool.hpp"

namespace Plazza
{
    class Kitchen
    {
        public:
            Kitchen(const Core &core);
            ~Kitchen();
            void refillStock();
            void enqueueCookTask(std::function<void()> task);
            // void startCooks();

        private:
            const Core &_core;
            ThreadPool _cookPool;
            std::map <Ingredients, int> _stock;
            sem_t _accessStock;
            int _clock;
    };
};

#endif /* !KITCHEN_HPP_ */
