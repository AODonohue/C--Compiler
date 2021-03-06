/*********************************************************************
 * FILE NAME: SymbolTable.c
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: Functions to create and manipulate symbol tables.
 *********************************************************************/
#include "globals.h"
#include "SymbolTable.h"


SymbolTable *tables = NULL;
FunSymbol *funs = NULL;
SymbolTable *CompoundST;
SymbolTable *ParamST;


int hash (char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}


void initTable() {
    CompoundST = newSymbolTable(LOCAL);
    ParamST = newSymbolTable(PARAM);
    tables = newSymbolTable(GLOBAL);
    putFunction("input", ParamST, 0, TYPE_INTEGER);
    ParamST = newSymbolTable(PARAM);
    pushTable(ParamST);
    putVariable("i", PARAM, ParamST->size++, TYPE_INTEGER);
    popTable();
    putFunction("output", ParamST, ParamST->size, TYPE_VOID);
    ParamST = newSymbolTable(PARAM);
}


SymbolTable *newSymbolTable(Scope scope) {
    int i;

    SymbolTable *st = (SymbolTable *)malloc(sizeof(SymbolTable));
    ASSERT(st != NULL) {
        fprintf(stderr, "Failed to malloc for symbal table.\n");
    }
    st->scope = scope;
    st->size = 0;
    st->next = NULL;
    st->varList = NULL;
    for(i = 0; i<SIZE; i++) {
        st->hashTable[i] = NULL;
    }
    return st;
}


SymbolTable *topTable() {
    return tables;
}


SymbolTable *popTable() {
    ASSERT(tables != NULL) {
        fprintf(stderr, "Pop an empty table list.\n");
    }
    SymbolTable *st = tables;
    tables = tables->next;
    return st;
}


void pushTable(SymbolTable *st) {
    ASSERT(st != NULL) {
        fprintf(stderr, "Push an null table.\n");
    }
    st->next = tables;
    tables = st;
}


VarSymbol *getTopVar(char *name) {
    if(tables == NULL)
        return NULL;

    VarSymbol *l;
    int h = hash(name);
    for(l = tables->hashTable[h]; l!=NULL; l=l->next) {
        if(strcmp(l->name, name) == 0)
            break;
    }
    return l;
}


VarSymbol *getVariable(char *name) {
    if(tables == NULL)
        return NULL;

    int h = hash(name);
    SymbolTable *st;
    VarSymbol *l;
    for(st = tables; st!=NULL; st=st->next) {
        for(l = st->hashTable[h]; l!=NULL; l=l->next) {
            if(strcmp(l->name, name)==0)
                return l;
        }
    }
    return NULL;
}


FunSymbol *getFunction(char *name) {
    if(funs == NULL)
        return NULL;

    FunSymbol *fs;
    for(fs=funs; fs!=NULL; fs = fs->next) {
        if(strcmp(fs->name, name)==0)
            break;
    }
    return fs;
}


int putVariable(char *name, Scope scope, int offset, ExpType type) {
    VarSymbol *l, *tmp;
    int h = hash(name);

    if(tables == NULL) {
        l = NULL;
    } else {
        l =  tables->hashTable[h];
        while ((l != NULL) && (strcmp(name,l->name) != 0)) {
            l = l->next;
        }
    }

    if (l != NULL) {
        fprintf(stderr, "Duplicate declarations of variable: %s.\n", name);
        return 1;
    }

    l = (VarSymbol *) malloc(sizeof(VarSymbol));
    ASSERT(l != NULL) {
        fprintf(stderr, "Failed to malloc for VarSymbol.\n");
    }
    l->name = strdup(name);
    l->scope = scope;
    l->type = type;
    l->offset = offset;
    l->next = tables->hashTable[h];
    tables->hashTable[h] = l;
    l->next_FIFO = NULL;
    if(tables->varList == NULL) {
        tables->varList = l;
    } else {
        for(tmp=tables->varList; tmp->next_FIFO != NULL; tmp = tmp->next_FIFO);
        tmp->next_FIFO = l;
    }
    return 0;
}


int putFunction(char *name, SymbolTable *st, int num, ExpType type) {
    FunSymbol *fs;

    if(getFunction(name) != NULL) {
        fprintf(stderr, "Duplicate declarations of function: %s\n", name);
        return 1;
    }
    fs = (FunSymbol *)malloc(sizeof(FunSymbol));
    ASSERT(fs != NULL) {
        fprintf(stderr, "Failed to malloc for FunSymbol.\n");
    }
    fs->name = strdup(name);
    fs->type = type;
    fs->paramNum = num;
    fs->symbolTable = st;
    fs->next = funs;
    funs = fs;

    return 0;
}


void printSymTab(SymbolTable *st) {
    int i;

    fprintf(listing,"Variable Name  Offset\n");
    fprintf(listing,"-------------  ------\n");
    VarSymbol *vs = NULL;
    for (i=0; i<SIZE; ++i) {
        for(vs = st->hashTable[i]; vs != NULL; vs=vs->next) {
            fprintf(listing, "%-14s", vs->name);
            fprintf(listing, "%-8d", vs->offset);
            fprintf(listing, "\n");
        }
    }
    fprintf(listing, "\n");
}
