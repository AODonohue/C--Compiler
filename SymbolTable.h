/*********************************************************************
 * FILE NAME: SymbolTable.h
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: SymbolTable.c public interface.
 *********************************************************************/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "globals.h"


/*********************************************************************
 * FUNCTION NAME: hash
 * PURPOSE: Finds a value in the hash table
 * ARGUMENTS: The key to the value (char *) 
 * RETURNS: The corresponding value (int)
 *********************************************************************/
int hash(char *key);


/*********************************************************************
 * FUNCTION NAME: initTable
 * PURPOSE: Initializes all values in the symbol table.
 *********************************************************************/
void initTable();


/*********************************************************************
 * FUNCTION NAME: newSymbolTable
 * PURPOSE: Creates a new symbol table
 * ARGUMENTS: The scope of the new table (scope)
 * RETURNS: The newly created table (SymbolTable *)
 *********************************************************************/
SymbolTable *newSymbolTable(Scope scope);


/*********************************************************************
 * FUNCTION NAME: topTable
 * PURPOSE: Finds the table at the highest scope
 * RETURNS: The symbol table (SymbolTable *)
 *********************************************************************/
SymbolTable *topTable();


/*********************************************************************
 * FUNCTION NAME: popTable
 * PURPOSE: Finds next table in scope
 * RETURNS: The symbol table (SymbolTable *)
 *********************************************************************/
SymbolTable *popTable();


/*********************************************************************
 * FUNCTION NAME: pushTable
 * PURPOSE: Puts a table into the current lowest scope
 * ARGUMENTS: The table to be pushed (SymbolTable *)
 *********************************************************************/
void pushTable(SymbolTable *st);


/*********************************************************************
 * FUNCTION NAME: getTopVar
 * PURPOSE: Finds variable at highest scope
 * ARGUMENTS: The variable to be found (char *)
 * RETURNS: The corresponding variable (VarSymbol *)
 *********************************************************************/
VarSymbol *getTopVar(char *name);


/*********************************************************************
 * FUNCTION NAME: getVariable
 * PURPOSE: Finds a variable
 * ARGUMENTS: The variable to be found (char *)
 * RETURNS: The corresponding variable (VarSymbol *)
 *********************************************************************/
VarSymbol *getVariable(char *name);


/*********************************************************************
 * FUNCTION NAME: getFuntion
 * PURPOSE: Finds a function
 * ARGUMENTS: The function to be found (char*)
 * RETURNS: The corresponding function (FunSymbol *)
 *********************************************************************/
FunSymbol *getFunction(char *name);


/*********************************************************************
 * FUNCTION NAME: putVariable
 * PURPOSE: Places a variable into the symbol table
 * ARGUMENTS: . The name of the variable (char *) 
 *            . The scope of the variable (Scope)
 *            . The value of the variable (int)
 *            . The data type of the variable (ExpType)
 * RETURNS: True (a nonzero integer) if the variable already exists,
 *          false (0) otherwise
 *********************************************************************/
int putVariable(char *name, Scope s, int offset, ExpType type);


/*********************************************************************
 * FUNCTION NAME: putFunction
 * PURPOSE: Places a function into the symbol table
 * ARGUMENTS: . The name of the function (char *) 
 *            . The symbol table to insert the function into 
 *              (SymbolTable)
 *            . The value of the function (int)
 *            . The data type of the function (ExpType)
 * RETURNS: True (a nonzero integer) if the variable already exists,
 *          false (0) otherwise
 *********************************************************************/
int putFunction(char *name, SymbolTable *st, int num, ExpType type);


/*********************************************************************
 * FUNCTION NAME: printSymTab
 * PURPOSE: Prints symbol table to stdout
 * ARGUMENTS: The table to be printed (SymbolTable *)
 *********************************************************************/
void printSymTab(SymbolTable *st);


#endif
