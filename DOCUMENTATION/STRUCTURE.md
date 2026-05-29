# Project Structure - Plazza

## Complete Directory Tree

```
G-CCP-400-LIL-4-1-theplazza-23/
│
├── Makefile                    # Build configuration file
├── README.md                   # Project overview
├── INSTALL.md                  # Installation and dependency guide
├── USAGE.md                    # User manual and command reference
├── ARCHITECTURE.md             # Technical architecture documentation
├── STRUCTURE.md                # This file
│
└── src/                        # Source code directory
    │
    ├── main.cpp                # Program entry point
    │
    ├── Core/                   # Core system orchestration
    │   ├── Core.hpp            # Core class header
    │   └── Core.cpp            # Core class implementation
    │                           
    ├── Parser/                 # Command parsing system
    │   ├── Parser.hpp          # Parser class header
    │   └── Parser.cpp          # Parser class implementation
    │                           
    ├── Producer/               # Reception/order producer
    │   ├── Producer.hpp        # Producer class header
    │   └── Producer.cpp        # Producer class implementation
    │                           
    ├── Consumer/               # Process/kitchen consumer
    │   ├── Consumer.hpp        # Consumer class header
    │   └── Consumer.cpp        # Consumer class implementation
    │                           
    ├── Kitchen/                # Kitchen and cooking system
    │   ├── Kitchen.hpp         # Kitchen class header
    │   ├── Kitchen.cpp         # Kitchen class implementation
    │   │
    │   └── Recipe/             # Pizza recipe system
    │       ├── IRecipe.hpp     # Abstract recipe interface
    │       ├── MargaritaRecipe.hpp
    │       ├── MargaritaRecipe.cpp
    │       ├── ReginaRecipe.hpp
    │       ├── ReginaRecipe.cpp
    │       ├── AmericanaRecipe.hpp
    │       ├── AmericanaRecipe.cpp
    │       ├── FantasiaRecipe.hpp
    │       └── FantasiaRecipe.cpp
    │
    ├── Utils/                  # Utility classes and helpers
    │   └── SafeQueue/          # Thread-safe queue implementation
    │       ├── ISafeQueue.hpp  # Safe queue interface
    │       └── SafeQueue.hpp   # Safe queue implementation
    │
    └── Include/                # Global headers and utilities
        └── RTCError.hpp        # Error handling definitions
```

## Component Details

### Main Entry Point

#### `main.cpp`
- **Purpose**: Program initialization and CLI loop
- **Responsibilities**:
  - Parse command-line arguments (multiplier, cooks, refresh interval)
  - Initialize Core system
  - Launch interactive command loop
  - Handle user input
  - Graceful shutdown
  
- **Function Signature**:
```cpp
int main(int argc, char **argv)
```

- **Command-Line Arguments**:
  1. `argv[1]` - Cooking time multiplier (float)
  2. `argv[2]` - Number of cooks per kitchen (int)
  3. `argv[3]` - Ingredient regeneration interval in ms (int)

---

### Core System

#### `Core/Core.hpp` & `Core/Core.cpp`
- **Purpose**: Central orchestrator of the entire system
- **Key Responsibilities**:
  - Initialize all subsystems
  - Manage command dispatching
  - Coordinate between reception and kitchens
  - Handle system shutdown
  
- **Class Members**:
```cpp
class Core {
    std::vector<Kitchen> kitchens;        // Active kitchens
    Producer reception;                   // Order reception
    float cookingMultiplier;              // Cooking time factor
    uint32_t regenerationInterval;        // Ingredient refresh rate
    bool running;                         // System state
};
```

- **Key Methods**:
  - `void initialize(float mult, int cooks, uint32_t refresh)`
  - `void processCommand(const std::string& cmd)`
  - `void displayStatus()`
  - `void shutdown()`

---

### Parser System

#### `Parser/Parser.hpp` & `Parser/Parser.cpp`
- **Purpose**: Validate and extract pizza order information
- **Responsibilities**:
  - Tokenize user input
  - Validate against grammar rules
  - Extract pizza type, size, and quantity
  - Provide detailed error messages

- **Class Members**:
```cpp
class Parser {
    std::vector<std::string> tokens;      // Parsed tokens
    size_t currentPos;                    // Current token position
};
```

- **Key Methods**:
  - `bool parse(const std::string& input)`
  - `std::vector<Order> getOrders()`
  - `std::string getErrorMessage()`
  - `bool validateType(const std::string& type)`
  - `bool validateSize(const std::string& size)`

- **Grammar Validation**:
  - Type: Pizza name (regina, margarita, etc.)
  - Size: S, M, L, XL, XXL (uppercase)
  - Quantity: x[1-99] format

---

### Reception/Producer

#### `Producer/Producer.hpp` & `Producer/Producer.cpp`
- **Purpose**: Receive orders and distribute them to kitchens
- **Responsibilities**:
  - Accept validated pizza orders
  - Implement load balancing algorithm
  - Create new kitchens when needed
  - Send orders via IPC to kitchens
  - Track order status

- **Class Members**:
```cpp
class Producer {
    std::vector<std::shared_ptr<Kitchen>> kitchens;
    std::queue<Order> pendingOrders;      // Orders awaiting kitchens
    float cookingMultiplier;
    LoadBalancer loadBalancer;
};
```

- **Key Methods**:
  - `void receiveOrder(const std::vector<Order>& orders)`
  - `Kitchen* selectBestKitchen(const Order& order)`
  - `void createKitchen()`
  - `void distributeOrder(const Order& order, Kitchen* kitchen)`
  - `void onOrderComplete(const OrderCompletion& completion)`

- **Load Balancing Algorithm**:
  1. Find kitchen with minimum current load
  2. If all kitchens at capacity (> 2×N pizzas):
     - Create new kitchen
     - Assign to new kitchen
  3. Send pizza order to selected kitchen

---

### Kitchen/Consumer

#### `Consumer/Consumer.hpp` & `Consumer/Consumer.cpp`
- **Purpose**: Manage kitchen processes and lifecycle
- **Responsibilities**:
  - Wrap fork/wait system calls
  - Manage child process communication
  - Detect process termination
  - Handle graceful shutdown

- **Class Members**:
```cpp
class Consumer {
    pid_t childPid;                       // Kitchen process ID
    int communicationFd;                  // IPC file descriptor
    bool isRunning;
};
```

- **Key Methods**:
  - `bool forkKitchen()`
  - `void sendOrder(const Pizza& pizza)`
  - `OrderCompletion receiveCompletion()`
  - `bool isAlive()`
  - `void terminate()`

#### `Kitchen/Kitchen.hpp` & `Kitchen/Kitchen.cpp`
- **Purpose**: Core kitchen logic (runs in child process)
- **Responsibilities**:
  - Initialize cook thread pool
  - Manage ingredient stock
  - Orchestrate pizza cooking
  - Report completions to reception
  - Implement timeout detection
  - Handle graceful shutdown

- **Class Members**:
```cpp
class Kitchen {
    std::vector<std::thread> cooks;       // Worker threads
    SafeQueue<Pizza> workQueue;           // Pizza assignments
    IngredientStock stock;                // Ingredient inventory
    std::atomic<uint32_t> lastActivityTime;
    float cookingMultiplier;
};
```

- **Key Methods**:
  - `void initialize(int numCooks, float multiplier)`
  - `bool canAcceptOrder(const Pizza& pizza)`
  - `void queuePizza(const Pizza& pizza)`
  - `void cookLoop()`
  - `void checkTimeout()`
  - `bool tryConsumeIngredients(const std::vector<Ingredient>& needed)`

- **Thread Pool Pattern**:
  - N cook threads
  - Shared work queue
  - Mutex-protected stock
  - Condition variable for wake-ups

---

### Recipe System

#### `Recipe/IRecipe.hpp`
- **Purpose**: Abstract interface for all pizza recipes
- **Interface**:
```cpp
class IRecipe {
public:
    virtual ~IRecipe() = default;
    virtual std::vector<Ingredient> getIngredients() const = 0;
    virtual uint32_t getCookingTime(float multiplier) const = 0;
    virtual PizzaType getType() const = 0;
};
```

#### `Recipe/MargaritaRecipe.hpp/cpp`
- **Type**: Margarita (enum value = 2)
- **Ingredients**: Dough, Tomato, Gruyere
- **Base Cooking Time**: 1 second

#### `Recipe/ReginaRecipe.hpp/cpp`
- **Type**: Regina (enum value = 1)
- **Ingredients**: Dough, Tomato, Gruyere, Ham, Mushrooms
- **Base Cooking Time**: 2 seconds

#### `Recipe/AmericanaRecipe.hpp/cpp`
- **Type**: Americana (enum value = 4)
- **Ingredients**: Dough, Tomato, Gruyere, Steak
- **Base Cooking Time**: 2 seconds

#### `Recipe/FantasiaRecipe.hpp/cpp`
- **Type**: Fantasia (enum value = 8)
- **Ingredients**: Dough, Tomato, Eggplant, Goat Cheese, Chief Love
- **Base Cooking Time**: 4 seconds

---

### Utilities

#### `Utils/SafeQueue/ISafeQueue.hpp`
- **Purpose**: Thread-safe queue interface
- **Interface**:
```cpp
template<typename T>
class ISafeQueue {
public:
    virtual void push(const T& value) = 0;
    virtual bool pop(T& value) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
};
```

#### `Utils/SafeQueue/SafeQueue.hpp`
- **Purpose**: Concrete thread-safe queue implementation
- **Features**:
  - Mutex protection
  - Condition variables
  - Non-blocking pop operations
  - Exception safety
  
- **Implementation Pattern**:
```cpp
template<typename T>
class SafeQueue : public ISafeQueue<T> {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable condVar;
public:
    void push(const T& value) override
    bool try_pop(T& value)
    void notify_all()
};
```

---

### Error Handling

#### `Include/RTCError.hpp`
- **Purpose**: Define custom exception hierarchy
- **Exception Classes**:
```cpp
class RTCException : public std::exception
class ParseException : public RTCException
class IPCException : public RTCException
class ProcessException : public RTCException
class KitchenException : public RTCException
class TimeoutException : public RTCException
```

---

## Data Flow Diagrams

### Order Processing Flow

```
User Input
    ↓
main.cpp receives command
    ↓
Parser.parse(command)
    ↓ (validated orders)
Core.processCommand()
    ↓
Producer.receiveOrder()
    ↓
LoadBalancer.selectKitchen()
    ↓ (pizza per kitchen)
Producer.sendOrder()
    ↓ (IPC message)
Kitchen (child process)
    ↓
SafeQueue.push()
    ↓
Cook threads awakened
    ↓
Cook.cookPizza()
    ↓ (recipe lookup)
Check IngredientStock
    ↓ (consume ingredients)
sleep() (cooking simulation)
    ↓
Report completion
    ↓ (IPC message)
Producer.onOrderComplete()
    ↓
Display to user
    ↓
Log to file
```

### Inter-Process Communication

```
Reception (Parent)           Kitchen #1 (Child)
      │                            │
      ├─ send(Order 1)────────────>│
      │                     Queue.push()
      │                            │
      │                       Cook threads work
      │                            │
      │ receive(Completion 1)<────┤
      │                    send()  │
      │                            │
      ├─ send(Order 2)────────────>│
      │                            │
      └─ send(Kill)───────────────>│ exit()
```

---

## Build System

### `Makefile`

**Standard Make Rules**:
```makefile
all:      # Compile the project
clean:    # Remove object files
fclean:   # Remove everything including binary
re:       # Rebuild from scratch
```

**Key Variables**:
```makefile
CC = g++                      # Compiler
CXXFLAGS = -std=c++17 -Wall   # Compilation flags
LDFLAGS = -pthread            # Linking flags
SRCS = src/main.cpp ...       # Source files
OBJS = $(SRCS:.cpp=.o)        # Object files
EXEC = plazza                 # Output binary
```

---

## Dependency Graph

```
main.cpp
    ├── Core
    │   ├── Parser
    │   ├── Producer
    │   │   ├── Kitchen
    │   │   │   ├── SafeQueue
    │   │   │   └── Recipe (*)
    │   │   └── LoadBalancer
    │   └── Consumer
    │       └── Kitchen (*)
    └── RTCError
```

`(*)` - Used by multiple components

---

## File Size Guidelines

| File | Lines | Role |
|------|-------|------|
| main.cpp | 50-100 | Entry point only |
| Core.cpp | 150-250 | System orchestration |
| Parser.cpp | 100-200 | Grammar validation |
| Producer.cpp | 150-250 | Load balancing |
| Consumer.cpp | 80-150 | Process management |
| Kitchen.cpp | 200-300 | Cook coordination |
| Recipe*.cpp | 30-50 each | Recipe definitions |
| SafeQueue.hpp | 80-120 | Template implementation |

---

## Key Integration Points

### 1. **main.cpp ↔ Core**
- Initialization call
- Command loop dispatching

### 2. **Core ↔ Parser**
- Parse user input
- Return ordered pizzas

### 3. **Core ↔ Producer**
- Send validated orders
- Receive completion notifications

### 4. **Producer ↔ Consumer/Kitchen**
- IPC message passing
- Order delivery and completion

### 5. **Kitchen ↔ Recipe**
- Get ingredient requirements
- Calculate cooking time

### 6. **Kitchen ↔ SafeQueue**
- Queue pizza assignments
- Thread-safe work distribution

---

## Compilation Dependencies

### Headers Required at Compile Time
- Standard library: `<iostream>`, `<vector>`, `<thread>`, `<mutex>`, etc.
- POSIX: `<unistd.h>`, `<sys/wait.h>`, `<pthread.h>`
- Project: All `.hpp` files

### Libraries Required at Link Time
- pthread: `-pthread` or `-lpthread`
- C++ runtime library (automatic)

---

## Next Steps

1. **Study the Architecture**: Read [ARCHITECTURE.md](ARCHITECTURE.md)
2. **Review Usage**: Check [USAGE.md](USAGE.md) for command examples
3. **Build the Project**: `make && ./plazza 2 5 2000`
4. **Examine Code**: Start with [main.cpp](src/main.cpp), then Core
5. **Test Components**: Use `status` command to verify functionality

---

**Last Updated**: May 2026
