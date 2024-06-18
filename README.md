# Alpha-Compiler
Welcome to the Alpha-Compiler Project!
This Repository contains the source code for a college assigned compiler of a language called Alpha!

## Overview
Alpha-Compiler was written for a college assignment under fair use, specifically for educational purposes. It serves as a stepping stone for understanding the fundamental components that constitute a modern-day compiler and should not be regarded as a fully-fledged one.

## Introduction
The Alpha-Compiler reads source files written in the Alpha programming language, and compiles them into Alpha-Binary files. The Alpha Language Specification (ALS) are defined by the Alpha Virtual Machine (AVM) used to run the binary files produced. You may find some example Alpha programs inside the tests folder.

The compiler is a CLI tool writen in C without making any system calls.

## Installation

Clone the repository to your local machine using either the GitHub Desktop app or by using the following command:

```console
git clone https://github.com/gthe0/Alpha-Compiler
```

## Compilation
For compiling the project, you need to have installed
- flex
- bison
- A C compiler

For \*nix platform, a Makefile is being provided, so simply run:
```console
make
```

For Windows, first generate the c code from the lex.l file, then do the same for the parser.y file. Afterwards, compile and link all the object files, forming the executable.

## Usage
As mentioned previously, the Alpha-Compiler is a CLI tool, so to use it you can run the executable and pass an Alpha Source Code file to it.

You can also pass the flags given below:

### Flags
- s
- i
- o
- Ox

## Contributing

Contributions to this project are welcome! If you would like to contribute, please follow these steps:

- Fork the repository.
- Create a new branch for your feature or bug fix.
- Make your changes and commit them.
- Push your changes to your forked repository.
- Submit a pull request, explaining the changes you have made.

