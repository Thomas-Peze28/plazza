/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** ISafeQueue
*/

namespace Plazza
{
    class ISafeQueue
    {
    public:
        virtual ~ISafeQueue() = default;
        virtual void push(int value) = 0;
        virtual bool tryPop(int &value) = 0;
        virtual int pop() = 0;
    };
}