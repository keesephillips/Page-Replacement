# Page Replacement Simulator

## Developer: Keese Phillips

## About

A robust, userspace implementation of memory management simulations written in C. This project simulates the behavior of an operating system's paging system, managing page allocation, frame replacement, and fault tracking. It allows users to compare different page replacement strategies to understand their efficiency and overhead.

The system is designed to handle:
- Page Fault Simulation: Accurate tracking of page hits, misses, and total fault counts.
- Frame Management: dynamic allocation of physical memory frames.
- Algorithm Comparison: Implementation of standard strategies like FIFO, LRU, and Clock.
- Reference String Processing: parsing of page access sequences from user input or files.
- Performance Metrics: Calculation of hit ratios and fault rates for each run.

## Installation & Compilation
**Prerequisites:**
- GCC Compiler (or any standard C compiler)
- Make
- Linux/Unix environment (Ubuntu, MacOS, WSL)

**Build:** To compile the project, run the included Makefile:

```bash
make
```
**Clean:** To clean up build artifacts:
```bash
make clean
```
**Run:** To start the page replacement simulator, run the executable with the required arguments:
```bash
./vm <number of frames> <replacement policy> <input file>
```

## Supported Algorithms & Commands
- **Replacement Policies:**
    - `0`: ZERO - A simple policy that always chooses frame 0 for replacement.
    - `1`: FIFO (First-In-First-Out) - Replaces the oldest page in memory.
    - `2`: LRU (Least Recently Used) - Replaces the page that has not been used for the longest period.
    - `3`: CLOCK (Second Chance) - A variation of FIFO that gives pages a "second chance" based on a reference bit.

## File Structure
- `vm.c`: The main entry point. Handles user input, parses arguments, and drives the simulation loop.
- `pagetable.c`: Implements the core logic for page replacement algorithms (FIFO, LRU, CLOCK), page fault handling, and the Memory Management Unit (MMU).
- `pt_core.c`: Contains core functions for managing the page table and inverted page table.
- `disk.c`: Simulates a swap disk for paging.
- `list.c` / `list.h`: Implements a doubly linked list used by the replacement algorithms.
- `vm.h`: Header file for the virtual memory simulation.
- `Makefile`: Build script to compile the source code into the `vm` executable.

## Technical Details
- **Max Frames:** Configurable via command-line argument (1-256).
- **Max Pages:** Configurable in the input file (1-256).
- **Max Processes:** Configurable in the input file (1-256).


## Data Structures
- **Page Table:** A two-level array representing the page table for each process.
- **Inverted Page Table:** An array that maps physical frames to virtual pages.
- **Linked List:** Used for implementing FIFO, LRU, and CLOCK algorithms.
- **Input Format:** Accepts an input file with a specific format for defining the simulation parameters and memory access patterns.
- **Simulation Logic:** Determines "Hit" or "Miss" status for every page reference and updates internal counters accordingly.
