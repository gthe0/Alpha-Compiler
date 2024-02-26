# HY340 - PHASE 1

## Overview

This project is a simple implementation of a lexical analyzer using LEX.

## Contributors

- Niki Psoma (CSD5038)
- Georgios Theodorakis (CSD4881)
- Fotis Pelantakis (CSD4988)

## Project Contents

- `lex.l`: Lexical analyzer specification file.
- `al.h`, `alpha_token.h`: Libraries.
- `al.c`: Implementation of a "printer".
- `alpha_token.c`: Contains list implementation (create, insert, print) and basic struct.
- `Makefile`: Contains instructions for building the project.

## Building and Running

To build the project, run the following command:
- make flex (This will generate the lexer code from `lex.l`)
- make compile (This will compile all the `.c` files, and link them into an executable named `scanner.out`.)

To run the lexical analyzer on an input file, use the following command:
- ./bin/scanner.out INPUT_FILE

INPUT_FILE: File containing the input (It's self explanatory). If no input file is specified, the program will terminate.

If you want to generate an output file, please use redirection as in the following example:
- ./bin/scanner.out INPUT_FILE > OUT_FILE
