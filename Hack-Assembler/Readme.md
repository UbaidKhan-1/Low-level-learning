# Hack Assembler

A lightweight Hack assembler written from scratch in C as part of my study of the Nand2Tetris computer architecture.

The assembler translates Hack assembly language into 16-bit Hack machine code and writes the result to an output file.

## Hack Architecture

The Hack platform is a simple 16-bit computer architecture consisting primarily of:

- A 16-bit CPU
- Instruction memory (ROM)
- Data memory (RAM)
- Memory-mapped I/O

The CPU contains two programmer-accessible registers:

- **A register** — used for addresses and general computation
- **D register** — used for general computation

The ALU operates on the values in these registers and/or memory.

Hack instructions are always exactly 16 bits long.

### A-instructions

Used to load a value into the A register:

    @value

They have the format:

    0vvvvvvvvvvvvvvv

For example:

    @21

### C-instructions

Used for computation, storing results, and conditional jumps:

    dest=comp;jump

They have the format:

    111accccccdddjjj

where:

- `comp` specifies the ALU computation
- `dest` specifies where the result is stored
- `jump` specifies a conditional jump

For example:

    D=M+1
    D;JGT
    M=D

## Assembler Pipeline

The assembler processes source code through four main stages:

    Source .asm file
            |
            v
    Pre-processing
            |
            |-- remove comments
            |-- strip whitespace
            |-- resolve symbols
            v
        Parsing
            |
            v
      Binary Encoding
            |
            v
        Output .hack file

### Symbol Resolution

The assembler uses two passes to resolve symbols.

Labels such as:

    (LOOP)

are assigned their corresponding ROM instruction address.

Variables that have not already been defined are assigned RAM addresses starting at address `16`.

The predefined Hack symbols such as `R0`–`R15`, `SCREEN`, and `KBD` are also included in the symbol table.

## Implementation

The assembler is written in C and uses custom data structures from the surrounding `Low-level-learning` project, including a hash map for symbol and instruction lookup.

The implementation is intentionally kept relatively lightweight. Comments and descriptive naming are used throughout the source to make the code self-documenting.

## Usage

The assembler expects exactly two command-line arguments:

    ./assemble Source_file Out_file

For example:

    ./assemble program.asm program.hack

The generated `.hack` file contains one 16-bit machine instruction per line.

## Project Context

This project is part of my exploration of computer systems from the bottom up:

    Logic Gates
        ↓
    ALU
        ↓
    CPU
        ↓
    Machine Language
        ↓
    Assembler
        ↓
    Higher-level software