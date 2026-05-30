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
    template<typename T>
    class MessageQueue
    {
    public:

        MessageQueue(const std::string& name) : _name(name)
        {
            struct mq_attr attr;
            attr.mq_flags = 0;
            attr.mq_maxmsg = 10;
            attr.mq_msgsize = sizeof(T);
            attr.mq_curmsgs = 0;

            _mq = mq_open(_name.c_str(), O_CREAT | O_RDWR, 0666, &attr);
            if (!_mq) {
                ErrorMajor("Error creating message queue");
            }
        }

        ~MessageQueue()
        {
            if (_mq) {
                mq_close(_mq);
            }
        }

        bool sendMessage(const T& msg)
        {
            if (mq_send(_mq, reinterpret_cast<const char*>(&msg), sizeof(T), 0) == 0) {
                return true;
            }
            return false;
        }

        bool receiveMessage(T& msg)
        {
            ssize_t bytes_read = mq_receive(_mq, reinterpret_cast<char*>(&msg), sizeof(T), nullptr);
            return bytes_read == sizeof(T);
        }

        void closeQueue()
        {
            mq_unlink(_name.c_str());
        }
    
    private:
        std::string _name;
        mqd_t _mq;
    };
}