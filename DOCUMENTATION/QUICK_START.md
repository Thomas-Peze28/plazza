# Quick Start Guide - Plazza

## TL;DR

```bash
# 1. Install dependencies (Ubuntu/Debian)
sudo apt install build-essential g++ make

# 2. Build
make

# 3. Run
./plazza 2 5 2000

# 4. Try commands
> regina XXL x2; margarita M x1
> status
> exit
```

---

## 60-Second Setup

| Step | Command | What it does |
|------|---------|-------------|
| 1 | `cd G-CCP-400-LIL-4-1-theplazza-23` | Enter project |
| 2 | `make` | Compile all source files |
| 3 | `./plazza 2 5 2000` | Start reception |
| 4 | `regina M x1` | Place pizza order |
| 5 | `status` | Check kitchen status |
| 6 | `exit` | Close program |

---

## Command Line Parameters

```bash
./plazza <multiplier> <cooks> <refresh_ms>
```

- **multiplier**: 0.1 to 4+ (cooking time factor)
- **cooks**: 1 to 20 (threads per kitchen)
- **refresh_ms**: 100 to 10000 (ingredient refresh interval)

### Common Setups

```bash
# Fast testing
./plazza 0.1 2 100

# Standard
./plazza 2 5 2000

# Heavy load
./plazza 0.5 10 1000

# Slow simulation
./plazza 4 5 5000
```

---

## Pizza Ordering

### Syntax
```
TYPE SIZE NUMBER [; TYPE SIZE NUMBER]*
```

### Types
- `regina`
- `margarita`
- `americana`
- `fantasia`

### Sizes
- `S` - Small
- `M` - Medium
- `L` - Large
- `XL` - Extra Large
- `XXL` - Double Extra Large

### Quantities
- `x1` to `x99` (lowercase 'x')

### Examples

✅ Valid:
```
margarita M x1
regina XL x2; margarita S x1
fantasia L x3; americana M x2; regina XXL x1
```

❌ Invalid:
```
margarita medium x1          # Size must be S,M,L,XL,XXL
regina x2                    # Missing size
pizza M x1                   # Unknown type
regina XL 2                  # Use x2 not 2
```

---

## CLI Commands

| Command | What it does |
|---------|-------------|
| `<order>` | Place pizza order (see syntax above) |
| `status` | Show all kitchens and ingredient levels |
| `recipes` | Show available pizzas and recipes |
| `help` | Show all commands |
| `exit` / `quit` / `q` | Close program |

### Examples

```bash
# Place order
plazza> regina XXL x2; margarita S x1

# Check status
plazza> status
Kitchen #1: 5 pizzas (3 cooking, 2 waiting)

# View recipes
plazza> recipes

# Get help
plazza> help

# Exit
plazza> exit
```

---

## Build Commands

| Command | Purpose |
|---------|---------|
| `make` | Compile program |
| `make clean` | Remove .o files |
| `make fclean` | Remove .o files and binary |
| `make re` | Clean and rebuild |

---

## Troubleshooting

### Can't compile
```bash
# Check compiler installed
g++ --version

# Install compiler
sudo apt install build-essential
```

### Program crashes
```bash
# Try clean rebuild
make fclean && make

# Check syntax
./plazza 2 5 2000  # Not ./plazza (missing args)
```

### Kitchens not working
```bash
# View status
plazza> status

# Check ingredient levels
# If empty, may need to wait for regeneration
```

### Too many processes
```bash
# Kill all plazza processes
pkill -f plazza

# Or kill specific PID
kill -9 <PID>
```

---

## Key Parameters Explained

### Multiplier (1st parameter)
```
Cooking time = recipe base × multiplier

Examples:
  multiplier = 0.1  →  Margarita: 100ms
  multiplier = 1.0  →  Margarita: 1sec
  multiplier = 2.0  →  Margarita: 2sec
  multiplier = 4.0  →  Margarita: 4sec
```

**Use case**: Testing (low multiplier) vs realistic simulation (1.0+)

### Cooks per Kitchen (2nd parameter)
```
Max pizzas per kitchen = 2 × cooks

Examples:
  cooks = 1   →  Max 2 pizzas per kitchen
  cooks = 5   →  Max 10 pizzas per kitchen
  cooks = 10  →  Max 20 pizzas per kitchen
```

**Use case**: More cooks = higher capacity, more resource usage

### Ingredient Refresh (3rd parameter)
```
Every N milliseconds, each ingredient increases by 1

Examples:
  refresh_ms = 100   →  10 per second
  refresh_ms = 1000  →  1 per second
  refresh_ms = 5000  →  1 per 5 seconds
```

**Use case**: Low value = can make more pizzas, high value = scarce ingredients

---

## System Behavior

### Order Routing
1. User places order → Parsed by Parser
2. Producer distributes pizza by pizza
3. Each pizza → Least-loaded kitchen
4. Kitchen full? → New kitchen created
5. Pizza cooked → Reception notified
6. User sees completion

### Kitchen Lifecycle
```
Creation
    ↓
Ready for orders
    ↓
Cooking pizzas
    ↓
Idle 5 seconds?
    ↓
Graceful shutdown
```

### Ingredient System
- Each kitchen starts with 5 of each ingredient
- Cooking consumes ingredients
- Automatically regenerate every N ms
- Maximum stays at 5

---

## Performance Tips

### For Testing (Want Speed)
```bash
./plazza 0.1 3 100
# Fast cooking, reasonable cooks, quick refresh
```

### For Realistic Simulation
```bash
./plazza 1 5 2000
# Normal times, good performance, balanced refresh
```

### For Heavy Load
```bash
./plazza 0.5 10 1000
# Multiple kitchens needed, stress test
```

### For Debugging
```bash
./plazza 10 2 5000
# Slow everything down for easier observation
```

---

## Output Interpretation

### Status Output Example
```
Kitchen #1 (5 cooks)
  Busy: 3 cooks
  Idle: 2 cooks
  Queue: 4 pizzas waiting
  Load: 7/10 (7 pizzas max 10)
  Ingredients:
    dough: 3/5
    tomato: 2/5
    gruyere: 5/5
```

**What it means**:
- 3 out of 5 cooks are actively cooking
- 4 pizzas waiting for an available cook
- Cannot accept more than 3 more pizzas
- Low ingredients: dough and tomato being used heavily

### Order Completion
```
[14:23:45] ORDER #1 READY
  regina XXL x2 (8 seconds)
  margarita S x1 (2 seconds)
```

**What it means**:
- Order processed in 8 seconds (longest pizza)
- All pizzas completed and ready

---

## File Locations

| What | Where |
|------|-------|
| Main program | `./plazza` |
| Source code | `./src/` |
| Makefile | `./Makefile` |
| README | `./README.md` |
| This guide | `./QUICK_START.md` |
| Installation help | `./INSTALL.md` |
| Usage guide | `./USAGE.md` |
| Architecture | `./ARCHITECTURE.md` |
| Full structure | `./STRUCTURE.md` |

---

## Next Steps

1. ✅ **Installation**: See [INSTALL.md](INSTALL.md)
2. ✅ **First Run**: `./plazza 2 5 2000`
3. ✅ **Learn Commands**: Type `help` in the program
4. 📖 **Deep Dive**: Read [USAGE.md](USAGE.md)
5. 🏗️ **Architecture**: Study [ARCHITECTURE.md](ARCHITECTURE.md)
6. 📁 **Project Layout**: Check [STRUCTURE.md](STRUCTURE.md)

---

## Common Issues Quick Fix

| Problem | Solution |
|---------|----------|
| `command not found: plazza` | Run `make` first to compile |
| `command not found: make` | Install: `sudo apt install make` |
| `Error: too many open files` | Run: `ulimit -n 4096` |
| `Segmentation fault` | Try clean rebuild: `make fclean && make` |
| `Order not accepted` | Check syntax: `type S|M|L|XL|XXL` (uppercase) |
| `Kitchen stuck/frozen` | Quit program: `exit` then restart |

---

## Example Session

```bash
$ ./plazza 2 5 2000

Welcome to Plazza Pizzeria Simulation
Reception started with 5 cooks per kitchen
Ingredient refresh every 2000ms
Cooking time multiplier: 2x

plazza> regina XXL x1
[13:45:00] Order #1 received: regina XXL x1
[13:45:00] Kitchen #1 created (PID: 12345)
[13:45:00] Assigned to Kitchen #1

plazza> margarita M x2
[13:45:01] Order #2 received: margarita M x2
[13:45:01] Assigned to Kitchen #1 (load: 3/10)

plazza> status
=== SYSTEM STATUS ===
Active kitchens: 1
Kitchen #1:
  Cooks: 5 (Busy: 3, Idle: 2)
  Current load: 3/10
  Queue: 0 waiting
  Ingredients:
    dough: 3/5
    tomato: 4/5
    gruyere: 3/5

plazza> [13:45:10] ORDER #1 READY: regina XXL x1
[13:45:11] ORDER #2 READY: margarita M x2

plazza> exit
Shutting down kitchens...
Kitchen #1 closed
All orders completed. Thank you!
$
```

---

**Last Updated**: May 2026  
**For more help**: Read [README.md](README.md) or [USAGE.md](USAGE.md)
