# Low-Level Learning

A collection of projects, data structures, algorithms, and experiments I'm building while learning **C and low-level programming**.

This repository documents my progression from higher-level languages like Python and JavaScript toward understanding how software works closer to the machine.

The goal isn't just to learn C syntax. It's to develop a deeper understanding of **memory, data representation, algorithms, data structures, parsing, and systems-oriented programming** by implementing things myself.

---

## Repository Structure

The repository is divided into two main categories:

```text
Low-level-learning/
│
├── DSA/
│   ├── Graphs/
│   ├── Linear-Structures/
│   ├── Trees/
│   └── Utils/
│
├── Dict-Parser/
├── Task-Manager/
├── Text-Encryptor/
└── README.md
```

### DSA

`DSA/` contains my implementations and experiments with **data structures and algorithms in C**.

This isn't organized as a collection of standalone projects. It is an evolving library of implementations that I build while studying DSA.

Current areas include:

- **Linear Structures**
  - Linked lists
  - Stacks
  - Queues
  - Hashmaps
  - Other linear data structures
- **Trees**
- **Graphs**
- **Utils**

I don't intend to create a separate README for every individual data structure or algorithm. The code itself is kept organized by category.

### Projects

The root-level directories contain more complete programs built around particular concepts.

Current projects include:

- **Dict-Parser** — A custom dictionary-style syntax parser that stores parsed values in a heterogeneous hashmap.
- **Task-Manager** — A command-line task management program.
- **Text-Encryptor** — A text encryption project.

Each larger project can have its own README when additional documentation is useful.

---

## What I'm Learning

Through these implementations and projects, I'm focusing on:

- Pointers and memory management
- Dynamic memory allocation
- Structs and custom data types
- File I/O and persistence
- Data structures
- Algorithms
- Hashing
- Parsing and text processing
- Command-line program design
- Modular C programming
- Debugging memory-related problems
- Understanding how higher-level abstractions can be implemented from lower-level primitives

---

## Learning Philosophy

I try to build things rather than only study their implementations.

For data structures and algorithms, this means writing them myself and dealing with the problems that come with doing so in C:

- Memory ownership
- Allocation and deallocation
- Pointer manipulation
- Edge cases
- Data representation
- Performance
- Undefined behavior
- Debugging

For larger projects, I try to combine these concepts into something that actually uses the structures I've built.

The repository will therefore contain both **small implementations for learning** and **larger projects that put those implementations to use**.

---

## Current Projects

### Dict Parser

A custom dictionary-style parser written from scratch in C.

Example syntax:

```text
{
    name: 'Carla',
    age: 19,
    occupation: 'teacher',
    isMarried: false
}
```

The parser currently supports:

- Strings
- Integers
- Booleans

The parsed values are stored in a heterogeneous hashmap.

---

### Task Manager

A command-line task management program built while learning C, data management, and file persistence.

---

### Text Encryptor

A C project focused on text manipulation and encryption.

---

## DSA Progress

The `DSA` directory is continuously evolving as I implement more algorithms and data structures.

Rather than treating each implementation as a separate project, I keep them together as a growing collection that I can reuse in other projects.

For example, a data structure implemented under `DSA/Linear-Structures` can later become a dependency of another project in the repository.

---

## Why This Repository Exists

I'm using this repository as both a **learning record and a place to build things**.

It allows me to look back at how my understanding develops as I move from basic C programming toward more complex low-level and systems concepts.

The projects will become progressively more ambitious as I learn more.

---

## Languages & Tools

- **C**
- Clang / GCC
- Linux / Termux
- Git & GitHub

---

## Status

This repository is actively being developed.

Implementations may be incomplete, experimental, or later rewritten as my understanding improves. That's intentional — this is a learning repository, not a collection of polished production libraries.