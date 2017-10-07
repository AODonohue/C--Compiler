/*********************************************************************
 * FILE NAME: globals.h
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: Global variables and data stuctures.
 *********************************************************************/
#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define ASSERT(x) for(;!(x);assert(x))

#define SIZE 211
#define SHIFT 4
#define DEBUG_SYM

extern FILE *source;
extern FILE *listing;
extern FILE *code;

extern int Table;

#define MAXCHILDREN 4


typedef enum {GLOBAL, LOCAL, PARAM} Scope;

typedef enum {TYPE_INTEGER, TYPE_VOID, TYPE_ARRAY, TYPE_UNDEFINED} ExpType;

typedef enum { TYPE_AST, VARDEC_AST, ARRAYDEC_AST, FUNDEC_AST,
               FUNHEAD_AST, PARAMID_AST, PARAMARRAY_AST,
               COMPOUND_AST,
               EXPSTMT_AST, SELESTMT_AST, ITERSTMT_AST, RETSTMT_AST, ASSIGN_AST,
               EXP_AST, VAR_AST, ARRAYVAR_AST,
               FACTOR_AST,
               CALL_AST, NUM_AST
             } ASTType;

typedef struct var_symbol VarSymbol;
struct var_symbol {
    char *name;
    Scope scope;
    ExpType type;
    int offset;
    struct var_symbol *next;
    struct var_symbol *next_FIFO;
};

typedef struct symbol_table SymbolTable;
struct symbol_table {
    int size;
    Scope scope;
    VarSymbol *hashTable[SIZE];
    VarSymbol *varList;
    struct symbol_table *next;
};

typedef struct fun_symbol FunSymbol;
struct fun_symbol {
    char *name;
    ExpType type;
    int offset;
    int paramNum;
    SymbolTable *symbolTable;
    struct fun_symbol *next;
};

typedef struct ASTNode TreeNode;
struct ASTNode {
    int lineno;
    struct ASTNode *child[MAXCHILDREN];
    struct ASTNode *sibling;
    ASTType astType;
    ExpType type;
    struct {
        int op;
        int value;
        char *name;
    } attr;
    SymbolTable *symbolTable;
};


extern SymbolTable *tables;
extern FunSymbol *funs;
extern SymbolTable *CompoundST;
extern SymbolTable *ParamST;
extern TreeNode *ASTRoot;


#endif
