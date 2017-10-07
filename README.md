# C- Compiler

This is a C- compiler built in C using Bison and Yacc. This compiler converts with low-level, single file, C- programs into assembly. The compiler catches all syntactic errors as well as most semantic and runtime errors. To execute the outputted assembly code TM Simulator is suggested.

## How To Run

Once file is made in directory, use any of the following flags to compile a C- file:

### Generate Syntax Table

```bash
$ cm <c-file> -s
```
This will display the generated syntax table in stdout (assuming no errors are found).
### Generate Abstract Syntax Tree

```bash
$ cm <c-file> -a
```
This will display the generated abstract syntax tree in stdout (assuming no errors are found).
### Generate Assembly Code

```bash
$ cm <c-file> -c
```
This will create an assembly file with the same name as the inputted file (assuming no errors are found).

NOTE: All flags can be used in conjunction with any other flag.