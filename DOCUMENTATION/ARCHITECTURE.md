# Architecture Guide - Plazza

## System Overview

Plazza implements a distributed pizzeria simulation system using multiple processes and threads to simulate real-world concurrency challenges.

```
┌─────────────────────────────────────────────────────────┐
│                  RECEPTION (Main Process)               │
│  • Interactive CLI                                      │
│  • Load Balancing                                       │
│  • Kitchen Management                                   │
│  • Order Logging                                        │
└──────────┬──────────────────────────────────────────────┘
           │ IPC (Pipes/Sockets/Message Queues)
    ┌──────┴──────┬──────────┬──────────┐
    │             │          │          │
    ▼             ▼          ▼          ▼
┌────────┐   ┌────────┐ ┌────────┐ ┌────────┐
│Kitchen │   │Kitchen │ │Kitchen │ │Kitchen │ (Child Processes)
│ #1     │   │ #2     │ │ #3     │ │ #N     │
│        │   │        │ │        │ │        │
│ Cook 1 │   │ Cook 1 │ │ Cook 1 │ │ Cook 1 │
│ Cook 2 │   │ Cook 2 │ │ Cook 2 │ │ Cook 2 │ (Threads)
│ Cook 3 │   │ Cook 3 │ │ Cook 3 │ │ Cook 3 │
│ ...    │   │ ...    │ │ ...    │ │ ...    │
│ Cook N │   │ Cook N │ │ Cook N │ │ Cook N │
│        │   │        │ │        │ │        │
│ Stock  │   │ Stock  │ │ Stock  │ │ Stock  │ (Ingredient Management)
└────────┘   └────────┘ └────────┘ └────────┘
```

## Architectural Layers

### Layer 1: Reception (Main Process)

**Purpose**: Act as the system's entry point and orchestrator

**Responsibilities**:
- Parse user commands from CLI
- Validate pizza orders
- Implement load balancing algorithm
- Manage kitchen lifecycle (creation/destruction)
- Distribute pizzas across kitchens
- Log completed orders
- Display system status

**Key Components**:
- `Core.hpp/cpp` - Main orchestration logic
- `Parser.hpp/cpp` - Command parsing and validation
- `Producer.hpp/cpp` - Order reception and distribution
- Interactive command loop

**Concurrency Model**:
- Single main thread (CLI reading)
- Handles multiple child processes
- Non-blocking I/O for kitchen communication

### Layer 2: Kitchens (Child Processes)

**Purpose**: Execute actual pizza cooking with thread management

**Responsibilities**:
- Manage cook thread pool
- Handle ingredient stock
- Cook pizzas based on recipes
- Report completion to reception
- Monitor idle time and self-destruct if needed

**Key Components**:
- `Kitchen.hpp/cpp` - Kitchen orchestration
- `Consumer.hpp/cpp` - Process management
- Thread pool implementation
- Ingredient stock system

**Concurrency Model**:
- One process per kitchen
- N threads (cooks) per kitchen
- Shared ingredient stock protected by mutex

**Lifecycle**:
```
Creation → Load Ingredients → Start Cook Threads
    ↓
Waiting for Orders
    ↓
Cooking Pizzas ← (Repeats until idle)
    ↓
Idle Timeout? → Graceful Shutdown
```

### Layer 3: Cooks (Threads)

**Purpose**: Cook individual pizzas concurrently

**Responsibilities**:
- Wait for pizza assignments
- Check ingredient availability
- Cook pizzas (sleep simulation)
- Report completion
- Yield when idle

**Concurrency Model**:
- Threads managed by thread pool per kitchen
- Mutex-protected work queue
- Condition variables for wake-up signals

**State Machine**:
```
┌──────────────┐
│   IDLE       │
│ (Waiting)    │
└──────┬───────┘
       │ Pizza assigned
       ▼
┌──────────────┐
│   CHECKING   │
│ (Ingredients)│
└──────┬───────┘
       │ Ingredients OK
       ▼
┌──────────────┐
│   COOKING    │
│ (Sleeping)   │
└──────┬───────┘
       │ Done
       ▼
┌──────────────┐
│  COMPLETED   │
└──────────────┘
```

### Layer 4: Communication (IPC)

**Purpose**: Enable message passing between reception and kitchens

**Supported Mechanisms**:
1. **Pipes** (FIFO) - Simple one-way or bidirectional
2. **Sockets** (Unix domain) - More flexible
3. **Message Queues** - POSIX or System V

**Message Types**:

**Reception → Kitchen:**
```cpp
struct PizzaOrder {
    uint8_t pizza_type;    // Enum value
    uint8_t pizza_size;    // Enum value
    uint32_t cook_id;      // Assigned cook (if any)
    // Packed binary format
};
```

**Kitchen → Reception:**
```cpp
struct OrderCompletion {
    uint32_t order_id;     // Original order ID
    uint8_t pizza_type;    // Completed pizza type
    uint8_t pizza_size;    // Completed pizza size
    uint32_t timestamp;    // Completion time
    // Packed binary format
};
```

**Serialization**:
- Binary encoding to minimize size
- Operator overloads for pack/unpack
- Endianness consideration for portability

## Core Components

### 1. Parser (`Parser.hpp/cpp`)

**Functionality**:
- Tokenize user input
- Validate syntax against grammar
- Extract pizza type, size, quantity
- Error reporting

**Grammar**:
```
S := TYPE SIZE NUMBER [; TYPE SIZE NUMBER]*
TYPE := [a..zA..Z]+
SIZE := S|M|L|XL|XXL
NUMBER := x[1..9][0..9]*
```

**Example Parsing Flow**:
```
Input: "regina XL x2; margarita S x1"
  ↓
Tokenize: ["regina", "XL", "x2", ";", "margarita", "S", "x1"]
  ↓
Validate: Each token against grammar rules
  ↓
Extract: 
  - Order 1: Type=regina, Size=XL, Qty=2
  - Order 2: Type=margarita, Size=S, Qty=1
  ↓
Output: Parsed order structure
```

### 2. Producer (`Producer.hpp/cpp`)

**Functionality**:
- Receive parsed orders
- Implement load balancing
- Distribute pizzas to kitchens
- Create new kitchens as needed

**Load Balancing Algorithm**:
```
For each pizza in order:
  1. Find kitchen with minimum current load
  2. If all kitchens at capacity (> 2*N pizzas):
     a. Create new kitchen
     b. Assign to new kitchen
  3. Send pizza order to selected kitchen
  4. Log allocation
```

**Capacity Management**:
- Max pizzas per kitchen = 2 × (number of cooks)
- Prevents overloading individual kitchens
- Triggers dynamic scaling

### 3. Kitchen (`Kitchen.hpp/cpp`)

**Functionality**:
- Thread pool management
- Ingredient stock management
- Pizza cooking orchestration
- Timeout detection

**Stock System**:
```cpp
struct IngredientStock {
    std::map<IngredientType, uint32_t> amounts;  // Current stock
    std::map<IngredientType, uint32_t> max_stock; // Maximum (5 units)
    
    void regenerate() {
        for (auto& [ingredient, current] : amounts) {
            if (current < max_stock[ingredient]) {
                current++;
            }
        }
    }
};
```

**Thread Pool Pattern**:
```
┌─────────────────────────────────────────┐
│  Kitchen Thread Pool                    │
├─────────────────────────────────────────┤
│ ┌─────────┐ ┌─────────┐ ┌─────────┐    │
│ │ Cook #1 │ │ Cook #2 │ │ Cook #N │    │
│ └─────────┘ └─────────┘ └─────────┘    │
│      ▲           ▲           ▲          │
│      └─────┬─────┴─────┬─────┘          │
│            │           │                │
│      ┌─────▼───────────▼─────┐          │
│      │  Work Queue (Mutex)   │          │
│      │  • Pizza assignments  │          │
│      │  • Cond. Variables    │          │
│      └───────────────────────┘          │
└─────────────────────────────────────────┘
```

### 4. Recipe System (`IRecipe.hpp` + implementations)

**Recipe Interface**:
```cpp
class IRecipe {
public:
    virtual ~IRecipe() = default;
    virtual std::vector<IngredientType> getIngredients() = 0;
    virtual uint32_t getCookingTime(float multiplier) = 0;
    virtual PizzaType getType() = 0;
};
```

**Pizza Definitions**:

| Pizza | Ingredients | Base Time |
|-------|-------------|-----------|
| Margarita | dough, tomato, gruyere | 1 sec |
| Regina | dough, tomato, gruyere, ham, mushrooms | 2 sec |
| Americana | dough, tomato, gruyere, steak | 2 sec |
| Fantasia | dough, tomato, eggplant, goat cheese, chief love | 4 sec |

### 5. Safe Queue (`SafeQueue.hpp`)

**Thread-Safe Container**:
```cpp
template<typename T>
class SafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cond_var;
    
public:
    void push(const T& value);
    bool try_pop(T& value);
    bool empty() const;
    size_t size() const;
    void notify();  // Wake waiting threads
};
```

**Use Cases**:
- Pizza work queue per kitchen
- Order completion notifications
- Cross-thread communication

## Concurrency Patterns

### Pattern 1: Thread Pool

**Implementation**:
```cpp
class ThreadPool {
    std::vector<std::thread> threads;
    SafeQueue<Task> work_queue;
    std::atomic<bool> shutdown;
    
public:
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] { worker_loop(); });
        }
    }
    
private:
    void worker_loop() {
        while (!shutdown) {
            Task task;
            if (work_queue.try_pop(task)) {
                task.execute();
            } else {
                // Yield - sleep briefly
                std::this_thread::yield();
            }
        }
    }
};
```

### Pattern 2: Mutex-Protected Resource

**Ingredient Stock**:
```cpp
class IngredientStock {
    std::map<Ingredient, uint32_t> stock;
    mutable std::mutex mutex;
    
public:
    bool tryConsume(const std::vector<Ingredient>& needed) {
        std::lock_guard<std::mutex> lock(mutex);
        for (const auto& ingredient : needed) {
            if (stock[ingredient] < 1) return false;
        }
        for (const auto& ingredient : needed) {
            --stock[ingredient];
        }
        return true;
    }
};
```

### Pattern 3: Condition Variable Synchronization

**Cook Waiting for Work**:
```cpp
class Cook {
    SafeQueue<Pizza> work_queue;
    std::condition_variable cv;
    std::mutex mutex;
    
    void workLoop() {
        while (true) {
            Pizza pizza;
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this] { return !work_queue.empty() || shutdown; });
                if (!work_queue.try_pop(pizza)) continue;
            }
            cookPizza(pizza);
            notifyCompletion(pizza);
        }
    }
};
```

## Data Structures

### Enum: PizzaType

```cpp
enum PizzaType {
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};
```

### Enum: PizzaSize

```cpp
enum PizzaSize {
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};
```

### Enum: IngredientType

```cpp
enum IngredientType {
    Dough = 1,
    Tomato = 2,
    Gruyere = 4,
    Ham = 8,
    Mushrooms = 16,
    Steak = 32,
    Eggplant = 64,
    GoatCheese = 128,
    ChiefLove = 256
};
```

## Timing Model

### Cooking Time Calculation

```
Final Cooking Time = Base Recipe Time × Multiplier Parameter
```

**Examples** (with multiplier = 2):
- Margarita: 1 sec × 2 = 2 seconds
- Regina: 2 sec × 2 = 4 seconds
- Americana: 2 sec × 2 = 4 seconds
- Fantasia: 4 sec × 2 = 8 seconds

### Ingredient Regeneration

```
Regeneration Interval = 3rd Parameter (milliseconds)

Every N milliseconds:
  For each ingredient in stock:
    if (current < maximum):
      current += 1
```

**Timeline**:
```
Time 0ms:   All ingredients = 5
Time 1000ms: Regenerate (with 3rd param = 1000)
Time 2000ms: Regenerate
Time ...
```

### Kitchen Idle Timeout

```
Timeout = 5 seconds of inactivity

Timeline:
  Pizza assignment → Cooking → Completion → No new pizza
                                                     ↓ (5 seconds)
                                            Kitchen shutdown
```

## Process Management

### Fork-Based Architecture

```
Reception (Parent Process, PID=100)
    │
    ├─ Fork
    │   └─ Kitchen #1 (Child, PID=101)
    │        └─ Cook Thread 1
    │        └─ Cook Thread 2
    │        └─ Cook Thread N
    │
    ├─ Fork
    │   └─ Kitchen #2 (Child, PID=102)
    │        └─ Cook Thread 1
    │        └─ Cook Thread 2
    │        └─ Cook Thread N
    │
    └─ ... (more kitchens as needed)
```

### Process Lifecycle

**Kitchen Child Process**:
1. `fork()` creates child process
2. Child initializes ingredient stock
3. Child starts N cook threads
4. Child enters message loop (wait for orders)
5. Child cooks pizzas (threads work)
6. Idle timeout → `exit(0)` gracefully

**Reception Parent Process**:
1. `fork()` for each kitchen needed
2. `wait()` or `waitpid()` for status
3. Detect if child exited
4. May restart kitchen if needed
5. Cleanup on program exit

## IPC Communication Flow

### Order Delivery

```
Reception sends pizza order:
    pack(PizzaOrder) 
        → serialize to binary
        → send via pipe/socket
        → Kitchen receives
        → unpack(PizzaOrder)
        → Parse and queue
```

### Completion Notification

```
Kitchen completes pizza:
    pack(OrderCompletion)
        → serialize to binary  
        → send via pipe/socket
        → Reception receives
        → unpack(OrderCompletion)
        → Log and display
```

## Error Handling

### Error Categories

1. **Parse Errors**: Invalid command syntax → User error message
2. **Capacity Errors**: Kitchen full → New kitchen creation
3. **Ingredient Errors**: Not enough stock → Wait or reject
4. **Process Errors**: Fork failed → Log error
5. **IPC Errors**: Message delivery failed → Retry or recovery
6. **Timeout Errors**: Kitchen idle 5s → Auto-shutdown

### Exception Hierarchy

```cpp
class RTCException : public std::exception {};
class ParseException : public RTCException {};
class IPCException : public RTCException {};
class ProcessException : public RTCException {};
class TimeoutException : public RTCException {};
```

## Performance Considerations

### Scalability

- **Horizontal**: Add more kitchens (processes)
- **Vertical**: Add more cooks per kitchen (threads)

### Bottlenecks

1. **IPC Bandwidth**: Message passing overhead
2. **Ingredient Contention**: Multiple cooks accessing stock
3. **Cook Idle Time**: Too many cooks with few pizzas
4. **Memory**: Each process consumes ~5-10MB

### Optimization Strategies

- Reduce context switches: Balance cooks/pizzas
- Batch ingredient updates: Less lock contention
- Async IPC: Non-blocking message passing
- Load prediction: Pre-create kitchens

## Testing Strategy

### Unit Tests

- Parser validation
- Recipe ingredient verification
- Load balancing algorithm
- Safe queue operations

### Integration Tests

- Single kitchen operation
- Multi-kitchen load balancing
- Ingredient regeneration
- Kitchen timeout

### System Tests

- High-volume orders
- Rapid kitchen creation/destruction
- Long-running stability
- Resource cleanup

---

**Last Updated**: May 2026
