# Usage Guide - Plazza

## Starting the Program

### Basic Syntax

```bash
./plazza <multiplier> <cooks_per_kitchen> <ingredient_refresh_ms>
```

### Parameters Explained

| Parameter | Name | Type | Range | Description |
|-----------|------|------|-------|-------------|
| 1st | `multiplier` | Float | 0.0 - ∞ | Multiplies cooking time (< 1 = divisor) |
| 2nd | `cooks_per_kitchen` | Integer | 1+ | Number of cook threads per kitchen |
| 3rd | `ingredient_refresh_ms` | Integer | 1+ | Milliseconds between ingredient regeneration |

### Examples

```bash
# Standard setup: 2x cooking, 5 cooks per kitchen, 2s ingredient refresh
./plazza 2 5 2000

# Fast cooking: 0.5x multiplier (half time), 3 cooks, 1s refresh
./plazza 0.5 3 1000

# Slow cooking: 4x multiplier, 10 cooks, 5s refresh
./plazza 4 10 5000

# Minimum setup: fast cooking, 1 cook, immediate refresh
./plazza 0.1 1 100
```

## Interactive CLI Commands

Once the program is running, you'll see the prompt:

```
plazza>
```

### Command: Place an Order

**Syntax:**
```
TYPE SIZE NUMBER [; TYPE SIZE NUMBER]*
```

**Components:**

- **TYPE**: Pizza type (case-insensitive)
  - `regina`
  - `margarita`
  - `americana`
  - `fantasia`

- **SIZE**: Pizza size (uppercase)
  - `S` - Small
  - `M` - Medium
  - `L` - Large
  - `XL` - Extra Large
  - `XXL` - Double Extra Large

- **NUMBER**: Quantity format `x` followed by 1-99
  - `x1` through `x99`

### Valid Order Examples

```bash
# Single pizza type
plazza> regina XL x1
Order received: Regina XL x1

# Multiple pizzas of same type
plazza> margarita M x5
Order received: Margarita M x5

# Multiple types in one order (semicolon-separated)
plazza> regina XXL x2; fantasia M x3; margarita S x1
Order received: Regina XXL x2; Fantasia M x3; Margarita S x1

# Complex order
plazza> regina L x3; margarita XL x2; americana M x4; fantasia S x1
Order received: Regina L x3; Margarita XL x2; Americana M x4; Fantasia S x1

# Case-insensitive
plazza> REGINA xl x2; MarGarita s x1
Order received: Regina XL x2; Margarita S x1
```

### Invalid Order Examples

```bash
# Missing size
plazza> regina x2
ERROR: Invalid order format

# Missing quantity
plazza> regina XL
ERROR: Invalid order format

# Invalid size
plazza> regina Medium x1
ERROR: Invalid pizza size. Use: S, M, L, XL, XXL

# Invalid type
plazza> pizza S x1
ERROR: Unknown pizza type

# Invalid quantity format (no 'x')
plazza> regina XL 2
ERROR: Invalid quantity format

# Quantity out of range
plazza> regina XL x0
ERROR: Quantity must be between 1 and 99

plazza> regina XL x100
ERROR: Quantity must be between 1 and 99

# Extra spaces (usually fails)
plazza> regina  XL  x2
ERROR: Invalid order format

# Spaces around semicolon (may work or fail depending on parser)
plazza> regina XL x2 ; margarita S x1
```

### Command: View System Status

**Syntax:**
```
status
```

**Output:**
Displays current state of all kitchens, including:
- Number of active kitchens
- Number of cooks in each kitchen
- Current load (pizzas being cooked or waiting)
- Ingredient stock levels
- Kitchen idle time

**Example:**
```bash
plazza> status

=== PLAZZA STATUS ===
Active Kitchens: 2

Kitchen #1 (PID: 12345)
  Status: WORKING
  Cooks: 5 (Busy: 3, Idle: 2)
  Queue: 4 pizzas waiting
  Load: 7/10
  Ingredients: dough=5, tomato=3, gruyere=4, ham=2, mushrooms=5, steak=5, eggplant=5, goat_cheese=5
  Idle time: 0s

Kitchen #2 (PID: 12346)
  Status: WORKING
  Cooks: 5 (Busy: 2, Idle: 3)
  Queue: 2 pizzas waiting
  Load: 4/10
  Ingredients: dough=5, tomato=5, gruyere=5, ham=3, mushrooms=2, steak=4, eggplant=5, goat_cheese=5
  Idle time: 0s

Queue (Awaiting allocation): 0 pizzas
```

### Command: List Recipes

**Syntax:**
```
recipes
```

**Output:**
Displays all available pizza recipes with ingredients and cooking times.

**Example:**
```bash
plazza> recipes

=== AVAILABLE RECIPES ===

Margarita
  Ingredients: dough, tomato, gruyere
  Cooking time: 1 sec × multiplier
  
Regina
  Ingredients: dough, tomato, gruyere, ham, mushrooms
  Cooking time: 2 sec × multiplier
  
Americana
  Ingredients: dough, tomato, gruyere, steak
  Cooking time: 2 sec × multiplier
  
Fantasia
  Ingredients: dough, tomato, eggplant, goat cheese, chief love
  Cooking time: 4 sec × multiplier
```

### Command: Help

**Syntax:**
```
help
```

**Output:**
Displays all available commands and their syntax.

### Command: Exit

**Syntax:**
```
exit
quit
q
```

**Behavior:**
- Waits for all current pizzas to finish cooking
- Closes all kitchens gracefully
- Exits the program

**Example:**
```bash
plazza> exit
Shutting down...
Kitchen #1 closed
Kitchen #2 closed
All orders completed. Thank you!
```

## Order Processing Workflow

### Step 1: Submission
```
plazza> regina XXL x2; margarita S x1
```

### Step 2: Parsing
Parser validates:
- ✓ Pizza types are valid
- ✓ Sizes are valid (S, M, L, XL, XXL)
- ✓ Quantities are in range (1-99)

### Step 3: Distribution
Reception allocates pizzas to kitchens:
- Load balancing ensures balanced distribution
- New kitchen created if all existing kitchens are full (> 2×N pizzas)
- Each pizza allocated individually

### Step 4: Cooking
- Cook threads prepare pizzas according to recipes
- Ingredients consumed from kitchen stock
- Cooking time = recipe base time × multiplier

### Step 5: Completion
```
[13:45:22] ORDER #1234 READY: regina XXL x2 (ready), margarita S x1 (ready)
```
- Order logged to file
- User notified of completion

## Ingredient System

### Initial Stock (per kitchen)
- All ingredients start with **5 units**

### Ingredients

| Type | Used In | Initial | Regeneration |
|------|---------|---------|--------------|
| dough | All pizzas | 5 | 1/N ms |
| tomato | All pizzas | 5 | 1/N ms |
| gruyere | Margarita, Regina, Americana | 5 | 1/N ms |
| ham | Regina | 5 | 1/N ms |
| mushrooms | Regina | 5 | 1/N ms |
| steak | Americana | 5 | 1/N ms |
| eggplant | Fantasia | 5 | 1/N ms |
| goat_cheese | Fantasia | 5 | 1/N ms |
| chief_love | Fantasia | 5 | 1/N ms |

### Regeneration

- Happens automatically every N milliseconds (3rd parameter)
- Increases each ingredient by 1 unit
- Capped at initial stock level (5 units)
- Example: `./plazza 2 5 2000` regenerates every 2 seconds

## Kitchen Management

### Kitchen Capacity

- Maximum pizzas per kitchen = 2 × (number of cooks)
- Example: 5 cooks = max 10 pizzas
- Includes: pizzas being cooked + pizzas waiting to cook

### Kitchen Timeout

- Kitchen automatically closes if **idle for 5 seconds**
- "Idle" = no pizzas cooking or waiting
- Graceful shutdown with status logging

### Auto-Scaling

- New kitchen created when needed (all kitchens full)
- Kitchens run in separate child processes
- Communication via IPC (pipes/sockets/message queues)

## Performance Optimization

### Recommended Settings

**Fast iteration (testing):**
```bash
./plazza 0.1 2 100
```
- 10x faster cooking
- 2 cooks (low overhead)
- Fast ingredient refresh

**Realistic simulation:**
```bash
./plazza 1 5 2000
```
- Normal cooking times
- 5 cooks (balanced load)
- 2-second ingredient refresh

**Heavy load test:**
```bash
./plazza 0.5 10 1000
```
- Half cooking time
- 10 cooks per kitchen
- Frequent ingredient refresh

### Monitoring

During execution:
- Use `status` to check kitchen load
- Check system resources: `top`, `ps`
- Monitor log file for order completions

## Common Workflows

### Workflow 1: Single Order Processing

```bash
./plazza 2 5 2000

plazza> margarita L x1
[13:45:00] Order #1 received: Margarita L x1
[13:45:02] ORDER #1 READY: Margarita L x1

plazza> status
Kitchen #1: Load 0/10

plazza> exit
```

### Workflow 2: High-Volume Orders

```bash
./plazza 1 5 2000

plazza> regina XXL x5; margarita L x5; fantasia M x5
[13:45:00] Order #1 received
[13:45:02] Kitchen #1 created
[13:45:04] Kitchen #2 created
[13:45:08] ORDER #1 READY: All pizzas complete

plazza> status
# Shows multiple kitchens working in parallel

plazza> regina XL x10
[13:45:10] Order #2 received
[13:45:12] Kitchen #3 created (auto-scaling)

plazza> exit
```

### Workflow 3: Continuous Operation

```bash
./plazza 0.5 3 1000

plazza> regina M x2

# While cooking:
plazza> margarita S x3
[13:45:01] New order queued

plazza> status
# Shows both orders being processed

plazza> americana L x2
[13:45:02] New order queued

# Wait for completions...
[13:45:03] ORDER #1 READY
[13:45:04] ORDER #2 READY
[13:45:05] ORDER #3 READY

plazza> exit
```

## Troubleshooting

### Order Not Accepted

**Symptom:** Command entered but no response

**Solutions:**
1. Check syntax against examples above
2. Ensure no typos in pizza types
3. Verify sizes are uppercase (S, M, L, XL, XXL)
4. Check quantity format (e.g., `x5`, not `5x`)

### Kitchen Not Working

**Symptom:** Pizzas in queue but not cooking

**Solutions:**
1. Check ingredient levels: `status`
2. Verify cooks are available
3. Check for error messages in console
4. Restart with more cooks: `./plazza 2 10 2000`

### System Running Out of Memory

**Symptom:** "Cannot allocate memory" error

**Solutions:**
1. Reduce cook count: `./plazza 2 3 2000`
2. Close other applications
3. Check system resources: `free -h`

## Output Files

### Log File

Location: `./plazza_orders.log` (or specified in code)

Contains:
- Timestamp of order placement
- Order ID and details
- Completion time
- Total cooking duration

**Example:**
```
[2024-05-28 13:45:00] Order #1 received: regina XXL x2; margarita S x1
[2024-05-28 13:45:02] Kitchen #1 assigned: regina XXL x2
[2024-05-28 13:45:02] Kitchen #2 assigned: margarita S x1
[2024-05-28 13:45:08] ORDER #1 COMPLETED (8 seconds)
```

---

**Last Updated**: May 2026
