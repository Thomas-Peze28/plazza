# Visual representation - Plazza

**The Project**:
```
┌──────────────────────────────┐
│   RECEPTION (Main Process)   │
│  • Interactive CLI           │
│  • Load Balancing            │
│  • Kitchen Management        │
│  • Order Logging             │
└──────────┬───────────────────┘
           │ IPC (Pipes/Sockets/Message Queues)
    ┌──────┴──────┬──────────┐
    │             │          │
    ▼             ▼          ▼
┌────────┐   ┌────────┐ ┌────────┐
│Kitchen │   │Kitchen │ │Kitchen │ (Child Processes)
│ #1     │   │ #2     │ │ #N     │
│        │   │        │ │        │
│ Cook 1 │   │ Cook 1 │ │ Cook 1 │
│ Cook 2 │   │ Cook 2 │ │ Cook 2 │ (Threads)
│ ...    │   │ ...    │ │ ...    │
│ Cook N │   │ Cook N │ │ Cook N │
│        │   │        │ │        │
│ Stock  │   │ Stock  │ │ Stock  │ (Ingredient Management)
└────────┘   └────────┘ └────────┘
```
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
**Thread Pool Pattern**:
```
┌─────────────────────────────────────┐
│  Kitchen Thread Pool                │
├─────────────────────────────────────┤
│ ┌─────────┐ ┌─────────┐ ┌─────────┐ │
│ │ Cook #1 │ │ Cook #2 │ │ Cook #N │ │
│ └─────────┘ └─────────┘ └─────────┘ │
│      ▲           ▲           ▲      │
│      └─────┬─────┴─────┬─────┘      │
│            │           │            │
│      ┌─────▼───────────▼─────┐      │
│      │  Work Queue (Mutex)   │      │
│      │  • Pizza assignments  │      │
│      │  • Cond. Variables    │      │
│      └───────────────────────┘      │
└─────────────────────────────────────┘
```