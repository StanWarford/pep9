pep9
====

The Pep/9 computer is a 16-bit complex instruction set computer (CISC). It is designed to teach computer architecture and assembly language programming principles. Its instructions are based on an expanding opcode and are either unary (one byte) or nonunary (three bytes). The eight addressing modes and eight dot commands are designed for straightforward translation between C/C++ and assembly language.

Pep/9 assembler and simulator
=============================

The assembler features an integrated text editor, error messages in red type that are inserted within the source code at the place where the error is detected, student-friendly machine language object code in hexadecimal format, the ability to code directly in machine language, bypassing the assembler, and the ability to redefine the mnemonics for the unimplemented opcodes that trigger synchronous traps.

The simulator features simulated ROM that is not altered by store instructions, a small operating system burned into simulated ROM that includes a loader and a trap handler system, an integrated debugger that allows for break points, single step execution, CPU tracing, and memory tracing, the option to trace an application, the loader, or the operating system, the ability to recover from endless loops, and the ability to modify the operating system by designing new trap handlers for the unimplemented opcodes.

