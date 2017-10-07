/*********************************************************************
 * FILE NAME: CodeGeneration.h
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: CodeGeneration.c public interface
 *********************************************************************/
#ifndef CODEGENERATION_H
#define CODEGENERATION_H

#include "globals.h"

#define TraceCode TRUE

#define zero 0
#define ax 1
#define bx 2

#define sp 4
#define bp 5
#define gp 6
#define pc 7


/*********************************************************************
 * FUNCTION NAME: generateComment
 * PURPOSE: Generates a comment line in assembly
 * ARGUMENTS: The comment to be generated (char *) 
 *********************************************************************/
void generateComment(char *c);


/*********************************************************************
 * FUNCTION NAME: generateRegOnly
 * PURPOSE: Generates a register line in assembly
 * ARGUMENTS: . The operation (char *) 
 *            . The location of the register in memory (int)
 *            . The initial value of the register (int)
 *            . The offset of the register (int)
 *            . The name of the register (char *)
 *********************************************************************/
void generateRegOnly(char *op, int r, int s, int t, char *c);


/*********************************************************************
 * FUNCTION NAME: generateRegMem
 * PURPOSE: Generates a register line in memory
 * ARGUMENTS: . The operation (char *) 
 *            . The location of the register in memory (int)
 *            . The initial value of the register (int)
 *            . The offset of the register (int)
 *            . The name of the register (char *)
 *********************************************************************/
void generateRegMem(char *op, int r, int d, int s, char *c);


/*********************************************************************
 * FUNCTION NAME: generateSkip
 * PURPOSE: Generates skips in assembly
 * ARGUMENTS: The amount of skips (int)
 *********************************************************************/
int generateSkip(int howMany);


/*********************************************************************
 * FUNCTION NAME: generateRewind
 * PURPOSE: Generates a rewind line in assembly
 * ARGUMENTS: The location in memory to rewind to (int) 
 *********************************************************************/
void generateRewind(int loc);


/*********************************************************************
 * FUNCTION NAME: generateRestore
 * PURPOSE: Generates a restore in assembly
 *********************************************************************/
void generateRestore(void);


/*********************************************************************
 * FUNCTION NAME: generatePrelude
 * PURPOSE: Generates a prelude line in assembly
 *********************************************************************/
void generatePrelude();


/*********************************************************************
 * FUNCTION NAME: generateInput
 * PURPOSE: Generates an input line in assembly
 *********************************************************************/
void generateInput();


/*********************************************************************
 * FUNCTION NAME: generateOutput
 * PURPOSE: Generates an output line in assembly
 *********************************************************************/
void generateOutput();


/*********************************************************************
 * FUNCTION NAME: generateGetAddr
 * PURPOSE: Generates a load address command in assembly
 * ARGUMENTS: The variable address to load (VarSymbol *) 
 *********************************************************************/
void generateGetAddr(VarSymbol *var);


/*********************************************************************
 * FUNCTION NAME: generateFunCall
 * PURPOSE: Generates a series of assembly lines simulating a function
 *          call
 * ARGUMENTS: The function to call (FunSymbol *) 
 *********************************************************************/
void generateFunCall(FunSymbol *fun);


/*********************************************************************
 * FUNCTION NAME: recursiveGen
 * PURPOSE: Recursively generates assembly code given a syntax tree
 * ARGUMENTS: The syntax tree to be generated (TreeNode *) 
 *********************************************************************/
void recursiveGen(TreeNode *tree);


/*********************************************************************
 * FUNCTION NAME: generateCode
 * PURPOSE: The main driver function for assembly code generation
 *********************************************************************/
void generateCode();


#endif