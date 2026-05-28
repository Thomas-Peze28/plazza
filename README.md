# Plazza - Pizzeria Simulation

> A multi-process, multi-threaded pizzeria simulation system written in C++

## 🍕 Overview

Plazza is an advanced C++ project that simulates a complete pizzeria system with multiple kitchens, cooks, and dynamic load balancing. It demonstrates critical concepts in concurrent programming including inter-process communication (IPC), thread synchronization, and process management.

### Key Features

- **Multi-Process Architecture**: Dynamic kitchen creation as demand increases
- **Multi-Threaded Kitchens**: Each kitchen has configurable cooks managed by a thread pool
- **Load Balancing**: Intelligent pizza distribution across kitchens to ensure balanced occupancy
- **Inter-Process Communication**: Robust communication between reception and kitchens
- **Ingredient Management**: Dynamic ingredient stock with automatic replenishment
- **Interactive CLI**: Real-time command interface for placing orders and checking status

## 📋 Table of Contents

1. [Quick Start](#quick-start)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Project Structure](#project-structure)
5. [Architecture](#architecture)
6. [Build System](#build-system)
7. [Contributing](#contributing)

## 🚀 Quick Start

```bash
# Build the project
make

# Run the reception with:
# - 2x cooking time multiplier
# - 5 cooks per kitchen
# - 2000ms ingredient regeneration interval
./plazza 2 5 2000

# In the interactive shell, place an order:
> regina XXL x2; fantasia M x3; margarita S x1

# Check kitchen status:
> status

# Exit:
> exit
```

## 💾 Installation

### Prerequisites

- **C++17** or later compiler (g++ or clang++)
- **POSIX-compliant system** (Linux, macOS, BSD)
- GNU Make or CMake
- Standard development tools (git, make)

### Step-by-Step Installation

See [INSTALL.md](DOCUMENTATION/INSTALL.md) for detailed dependency installation and build instructions.

```bash
# Clone or navigate to the project directory
cd G-CCP-400-LIL-4-1-theplazza-23

# Build the project
make

# Clean build artifacts
make clean

# Full clean (including binary)
make fclean

# Rebuild from scratch
make re
```

## 📖 Usage

See [USAGE.md](DOCUMENTATION/USAGE.md) for comprehensive usage guide.

### Basic Commands

**Place an Order:**
```
regina XL x2; margarita M x1; fantasia XXL x3
```

**Check Status:**
```
status
```

**Exit Program:**
```
exit
```

### Command Syntax

```
TYPE SIZE NUMBER [; TYPE SIZE NUMBER]*
```

- **TYPE**: Pizza type (regina, margarita, americana, fantasia)
- **SIZE**: S, M, L, XL, or XXL
- **NUMBER**: Quantity (x1 to x99)

## 📁 Project Structure

```
plazza/
├── src/
│   ├── main.cpp                 # Entry point
│   ├── Core/                    # Core system logic
│   │   ├── Core.cpp
│   │   └── Core.hpp
│   ├── Parser/                  # Command parser
│   │   ├── Parser.cpp
│   │   └── Parser.hpp
│   ├── Producer/                # Pizza producer (reception)
│   │   ├── Producer.cpp
│   │   └── Producer.hpp
│   ├── Consumer/                # Consumer process management
│   │   ├── Consumer.cpp
│   │   └── Consumer.hpp
│   ├── Kitchen/                 # Kitchen logic and cooks
│   │   ├── Kitchen.cpp
│   │   ├── Kitchen.hpp
│   │   └── Recipe/
│   │       ├── IRecipe.hpp      # Recipe interface
│   │       ├── MargaritaRecipe.cpp
│   │       ├── ReginaRecipe.cpp
│   │       ├── AmericanaRecipe.cpp
│   │       └── FantasiaRecipe.cpp
│   ├── Utils/                   # Utility classes
│   │   └── SafeQueue/
│   │       ├── ISafeQueue.hpp
│   │       └── SafeQueue.hpp
│   └── Include/
│       └── RTCError.hpp         # Error handling
├── Makefile                     # Build configuration
└── README.md                    # This file
```

See [ARCHITECTURE.md](DOCUMENTATION/ARCHITECTURE.md) for detailed technical architecture.

## 🏗️ Architecture Overview

### System Components

1. **Reception (Main Process)**
   - Interactive CLI for order placement
   - Load balancing logic
   - Kitchen management
   - Order logging

2. **Kitchens (Child Processes)**
   - Ingredient stock management
   - Cook thread pool
   - Pizza cooking simulation
   - Status reporting

3. **Cooks (Threads)**
   - One pizza at a time
   - Scheduled by thread pool
   - Yield when idle

4. **Communication**
   - IPC between reception and kitchens
   - Serialized pizza data
   - Status updates

For detailed architecture documentation, see [ARCHITECTURE.md](DOCUMENTATION/ARCHITECTURE.md).

## 🔨 Build System

### Makefile Rules

- `make` - Compile the project
- `make clean` - Remove object files
- `make fclean` - Remove all build artifacts
- `make re` - Rebuild from scratch

### Supported Compilers

- g++ 9.0+
- clang++ 10.0+

## 📊 System Requirements

| Aspect | Requirement |
|--------|------------|
| OS | Linux, macOS, BSD |
| Language | C++17 |
| Compiler | GCC 9+ or Clang 10+ |
| Memory | ~50MB base + ~10MB per kitchen |
| Build Time | ~5-10 seconds |

## 🐛 Troubleshooting

### Common Issues

**Compilation fails with C++17 errors:**
- Ensure your compiler supports C++17: `g++ --version`
- Update your compiler: `sudo apt update && sudo apt upgrade`

**IPC communication errors:**
- Check system limits: `ulimit -a`
- Ensure sufficient file descriptors available

**Processes not terminating:**
- Use `pkill -f plazza` to force termination
- Check for zombie processes: `ps aux | grep plazza`

For more troubleshooting, see [INSTALL.md](DOCUMENTATION/INSTALL.md).

## 📚 Documentation Files

- [INSTALL.md](DOCUMENTATION/INSTALL.md) - Detailed installation and dependency setup
- [USAGE.md](DOCUMENTATION/USAGE.md) - Comprehensive usage guide
- [ARCHITECTURE.md](DOCUMENTATION/ARCHITECTURE.md) - Technical architecture and design
- [VISUAL.md](DOCUMENTATION/VISUAL.md) - Visual representation of the project

## 🎯 Learning Objectives

This project teaches:

- **Process Management**: fork(), wait(), exit()
- **Thread Synchronization**: pthread, mutexes, condition variables
- **IPC Mechanisms**: Pipes, message queues, or sockets
- **Load Balancing**: Dynamic resource allocation
- **Object-Oriented Design**: Encapsulation of system concepts
- **Concurrent Programming**: Race conditions, deadlocks, synchronization

## 📝 Notes

- Cooking times are multiplied by the first command-line parameter
- Ingredient stock regenerates every N milliseconds (third parameter)
- Maximum 2×N pizzas per kitchen (N = number of cooks)
- Kitchen timeout: 5 seconds of inactivity
- All ingredients start at 5 units

## 📄 License

This project is part of Epitech's curriculum.

---

**Project Version**: 2.4  
**Last Updated**: May 2026
