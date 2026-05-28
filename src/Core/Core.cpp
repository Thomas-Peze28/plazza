/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** Core
*/

#include "Core.hpp"

bool Plazza::Core::_signal = false;

/**
* Displays help information for the plazza.
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
* @return True if help was displayed, false otherwise.
**/
bool Plazza::Core::displayHelp(int argc, char *argv[])
{
    for (unsigned int i = 0; argv[i] != NULL; i++) {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            std::cout << "USAGE: ./plazza <multiplier> <cooks_per_kitchen> <stock_refresh_ms>\n"
                      << "  <multiplier> : float or int (0 < value, can be < 1 to divide cooking time)\n"
                      << "  <cooks_per_kitchen> : integer number of cooks per kitchen\n"
                      << "  <stock_refresh_ms> : integer milliseconds for ingredient replenishment" << std::endl;
            return true;
        }
    }
    if (argc != 4) {
        std::cout << "USAGE: ./plazza <multiplier> <cooks_per_kitchen> <stock_refresh_ms>" << std::endl;
        return true;
    }
    return false;
}

/**
* Handles the signal for interrupting the plazza.
* @param signal The signal received.
**/
void Plazza::Core::signalHandler(int signal)
{
    if (signal == SIGINT) {
        _signal = true;
    }
}

/**
* Runs the plazza with the given configuration.
* @return The result of the plazza execution.
**/
int Plazza::Core::run()
{
    std::signal(SIGINT, signalHandler);

    std::cout << "Plazza is running..." << std::endl;

    const std::size_t producerCount = 2;
    const std::size_t consumerCount = 2;
    const std::size_t itemsPerProducer = 10;

    for (std::size_t i = 0; i < producerCount; ++i) {
        _producerThreads.emplace_back([this, itemsPerProducer, i]() {
            for (std::size_t j = 0; j < itemsPerProducer; ++j) {
                if (_signal) break;
                int value = rand() % 100;
                _queue.push(value);
                std::cout << "Producer " << i << " produced: " << value << std::endl;
            }
        });
    }

    for (std::size_t i = 0; i < consumerCount; ++i) {
        _consumerThreads.emplace_back([this, i]() {
            while (!_signal) {
                int value;
                _queue.tryPop(value);
                if (value != -1) {
                    std::cout << "Consumer " << i << " consumed: " << value << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }

    for (auto &thread : _producerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    for (auto &thread : _consumerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return 0;
}