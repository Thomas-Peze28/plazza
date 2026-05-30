/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** MessageQueue
*/

#include "MessageQueue.hpp"

/**
* @brief Construct a new Message Queue
* @param name The name of the message queue (must start with a '/')
**/
Plazza::MessageQueue::MessageQueue(const std::string &name) : _name(name)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Order);
    attr.mq_curmsgs = 0;

    _mq = mq_open(_name.c_str(), O_CREAT | O_RDWR, 0644, &attr);

    if (name.empty() || _name[0] != '/') {
        ErrorMajor("Message queue name must start with a '/'.");
        exit(84);
    }
    if (!_mq) {
        ErrorMajor("Failed to create message queue.");
        exit(84);
    }
}

/**
* @brief Destroy the Message Queue
**/
Plazza::MessageQueue::~MessageQueue()
{
    if (_mq) {
        mq_close(_mq);
    }
}

/**
* @brief Send a message to the queue
* @param order The order to send
* @return true if the message was sent successfully, false otherwise
**/
bool Plazza::MessageQueue::sendMessage(const Order &order)
{
    if (mq_send(_mq, reinterpret_cast<const char*>(&order), sizeof(Order), 0) == -1) {
        ErrorMajor("Failed to send message.");
        return false;
    }
    return true;
}

/**
* @brief Receive a message from the queue
* @param order The order to receive
* @return true if the message was received successfully, false otherwise
**/
bool Plazza::MessageQueue::receiveMessage(Order &order)
{
    if (mq_receive(_mq, reinterpret_cast<char*>(&order), sizeof(Order), nullptr) == -1) {
        ErrorMajor("Failed to receive message.");
        return false;
    }
    return true;
}

/**
* @brief Close the message queue
**/
void Plazza::MessageQueue::closeQueue()
{
    if (_mq) {
        mq_unlink(_name.c_str());
    }
}