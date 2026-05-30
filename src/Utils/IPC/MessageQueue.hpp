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
#include <ctime>

#include "Parser.hpp"
#include "RTCError.hpp"

namespace Plazza
{
    template<typename T>
    class MessageQueue
    {
    public:

        /** 
        * Constructor for the MessageQueue class.
        * @param name The name of the message queue.
        **/
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

        /** 
        * Destructor for the MessageQueue class.
        **/
        ~MessageQueue()
        {
            if (_mq) {
                mq_close(_mq);
            }
        }

        /** 
        * Sends a message through the message queue.
        * @param msg The message to be sent.
        * @return True if the message was sent successfully, false otherwise.
        **/
        bool sendMessage(const T& msg)
        {
            if (mq_send(_mq, reinterpret_cast<const char*>(&msg), sizeof(T), 0) == 0) {
                return true;
            }
            return false;
        }

        /** 
        * Receives a message from the message queue.
        * @param msg The message to be received.
        * @return True if the message was received successfully, false otherwise.
        **/
        bool receiveMessage(T& msg)
        {
            ssize_t bytes_read = mq_receive(_mq, reinterpret_cast<char*>(&msg), sizeof(T), nullptr);
            return bytes_read == sizeof(T);
        }

        /** 
        * Receives a message from the message queue with a timeout.
        * @param msg The message to be received.
        * @param timeoutSeconds The timeout in seconds.
        * @return True if the message was received successfully, false otherwise.
        **/
        bool receiveMessageWithTimeout(T& msg, int timeoutSeconds)
        {
            struct timespec timeout;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += timeoutSeconds;

            ssize_t bytes_read = mq_timedreceive(_mq, reinterpret_cast<char*>(&msg), sizeof(T), nullptr, &timeout);
            return bytes_read == sizeof(T);
        }

        /** 
        * Closes the message queue.
        **/
        void closeQueue()
        {
            mq_unlink(_name.c_str());
        }
    
    private:
        std::string _name;
        mqd_t _mq;
    };
}