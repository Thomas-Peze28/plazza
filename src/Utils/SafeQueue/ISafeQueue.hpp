/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** ISafeQueue
*/

namespace Plazza
{
    template <typename Type>
    class ISafeQueue
    {
    public:
        virtual ~ISafeQueue() = default;
        virtual void push(Type value) = 0;
        virtual bool tryPop(Type &value) = 0;
        virtual Type pop() = 0;
        virtual void stop() = 0;
    };
}