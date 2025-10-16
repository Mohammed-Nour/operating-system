# Operating Systems Coursework

This repository contains the assignments and solutions I completed for my Operating Systems course. Over the semester, I gained hands-on experience with core OS concepts through programming exercises in C and shell scripting.

## Course Overview

The course provided a comprehensive introduction to operating systems, covering topics from basic shell commands to advanced memory management. Each week built upon the previous one, starting with foundational skills and progressing to complex system-level programming.

Key areas explored:

- Shell scripting and file system operations
- C programming fundamentals and data types
- Process creation and management
- Inter-process communication
- CPU scheduling algorithms
- Memory allocation strategies
- Virtual memory and paging systems

## Repository Structure

The repository is organized by week, with each week's folder containing:

- `exercises/`: The original problem statements and requirements
- `solutions/`: My implemented solutions in C, shell scripts, and supporting files

```text
operating-system/
├── README.md
├── week01/ (Introduction & Shell Scripting)
│   ├── exercises/
│   └── solutions/
├── week02/ (C Programming Basics)
│   ├── exercises/
│   └── solutions/
├── week03/ (Pointers & Memory)
│   ├── exercises/
│   └── solutions/
├── week04/ (Process Management)
│   ├── exercises/
│   └── solutions/
├── week05/ (Inter-Process Communication)
│   ├── exercises/
│   └── solutions/
├── week06/ (CPU Scheduling)
│   ├── exercises/
│   └── solutions/
├── week07/ (Memory Allocation)
│   ├── exercises/
│   └── solutions/
├── week08/ (Virtual Memory)
│   ├── exercises/
│   └── solutions/
└── week09/ (Advanced Memory Management)
    ├── exercises/
    └── solutions/
```

## Table of Contents

### [Week 1: Introduction & Shell Scripting](week01/)

Started with the basics of shell scripting. Learned to navigate file systems, create directories, and perform text manipulations using command-line tools.

### [Week 2: C Programming Basics](week02/)

Focused on fundamental C concepts including data types, their sizes, and maximum values. Got comfortable with basic I/O and variable handling.

### [Week 3: Pointers & Memory](week03/)

Explored pointer arithmetic, memory allocation, and array manipulation. Implemented algorithms like the Tribonacci sequence to understand dynamic memory.

### [Week 4: Process Management](week04/)

Dived into process creation using fork(). Learned about parent-child relationships, process IDs, and measuring execution times across different processes.

### [Week 5: Inter-Process Communication](week05/)

Worked with channels, publishers, and subscribers to understand how processes communicate. Implemented basic IPC mechanisms.

### [Week 6: CPU Scheduling](week06/)

Studied different scheduling algorithms including Round Robin and Shortest Job First. Created scheduler implementations to simulate CPU resource allocation.

### [Week 7: Memory Allocation](week07/)

Explored memory allocation strategies and implemented custom allocators to understand how operating systems manage heap memory.

### [Week 8: Virtual Memory](week08/)

Learned about Memory Management Units (MMU) and paging systems. Implemented virtual memory concepts and page replacement algorithms.

### [Week 9: Advanced Memory Management](week09/)

Built upon previous memory concepts with more advanced techniques and optimizations in memory management.

## Running the Solutions

Most solutions are standalone C programs or shell scripts. To compile and run C programs:

```bash
gcc program.c -o program
./program
```

Shell scripts can be run directly:

```bash
chmod +x script.sh
./script.sh
```

Note: Some programs may require specific input files or have dependencies on the system setup.
