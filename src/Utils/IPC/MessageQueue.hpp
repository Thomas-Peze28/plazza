/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** MessageQueue
*/

#pragma once

#include <iostream>
#include <string>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "Parser.hpp"
#include "RTCError.hpp"

namespace Plazza
{
    class MessageQueue
    {
    public:
        MessageQueue(const std::string &name);
        ~MessageQueue();

        bool sendMessage(const Order &order);
        bool receiveMessage(Order &order);
        void closeQueue();
    
    private:
        std::string _name;
        mqd_t _mq;
    };
}