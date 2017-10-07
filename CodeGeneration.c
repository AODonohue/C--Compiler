/*********************************************************************
 * FILE NAME: CodeGeneration.c
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: Functions to generate assembly code.
 *********************************************************************/
#include "globals.h"
#include "parse.h"
#include "SymbolTable.h"
#include "CodeGeneration.h"


TreeNode *paramStack[SIZE];
int top = 0;
int emitLoc = 0;
int highEmitLoc = 0;


int pushParam(TreeNode *param) {

    if(top == SIZE)
        return 1;
    paramStack[top++] = param;

    return 0;
}


TreeNode *popParam() {

    if(top == 0)
        return NULL;

    return paramStack[--top];
}


int generateSkip(int howMany) {
    int i = emitLoc;

    emitLoc += howMany;
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;

    return i;
}


void generateRewind(int loc) {

    if (loc > highEmitLoc)
        generateComment("BUG in generateRewind");
    emitLoc = loc;

}


void generateRestore(void) {
    emitLoc = highEmitLoc;
}


void generateComment(char *c) {

    if (TraceCode)
        fprintf(code,"* %s\n",c);
}


void generateRegOnly(char *op, int r, int s, int t, char *c) {

    fprintf(code,"%3d:  %5s  %d,%d,%d ",emitLoc++,op,r,s,t);
    if (TraceCode)
        fprintf(code,"\t%s",c);
    fprintf(code,"\n");
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;
}


void generateRegMem(char *op, int r, int d, int s, char *c) {

    fprintf(code,"%3d:  %5s  %d,%d(%d) ",emitLoc++,op,r,d,s);
    if (TraceCode)
        fprintf(code,"\t%s",c);
    fprintf(code,"\n");
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;
}


void generatePrelude() {

    if (TraceCode)
        generateComment("Begin prelude");
    generateRegMem("LD",gp,0,zero,"load from location 0");
    generateRegMem("ST",zero,0,zero,"clear location 0");
    generateRegMem("LDA",sp,-(topTable()->size),gp,"allocate for global variables");
    if (TraceCode)
        generateComment("End of prelude");
}


void generateInput() {

    if (TraceCode)
        generateComment("Begin input()");
    FunSymbol *fun = getFunction("input");
    fun->offset = generateSkip(0);
    generateRegOnly("IN",ax,0,0,"read input into ax");
    generateRegMem("LDA",sp,1,sp,"pop prepare");
    generateRegMem("LD",pc,-1,sp,"pop return addr");
    if (TraceCode)
        generateComment("End input()");
}


void generateOutput() {

    if (TraceCode)
        generateComment("Begin output()");
    FunSymbol *fun = getFunction("output");
    fun->offset = generateSkip(0);
    generateRegMem("LD",ax,1,sp,"load param into ax");
    generateRegOnly("OUT",ax,0,0,"output using ax");
    generateRegMem("LDA",sp,1,sp,"pop prepare");
    generateRegMem("LD",pc,-1,sp,"pop return addr");
    if (TraceCode)
        generateComment("End output()");
}


void generateGetAddr(VarSymbol *var) {

    switch(var->scope) {
    case GLOBAL:
        if(var->type == TYPE_ARRAY) {
            generateRegMem("LDA",bx,-(var->offset),gp,"get global array address");
        } else {
            generateRegMem("LDA",bx,-1-(var->offset),gp,"get global address");
        }
        break;
    case LOCAL:
        if(var->type == TYPE_ARRAY) {
            generateRegMem("LDA",bx,-(var->offset),bp,"get local array address");
        } else {
            generateRegMem("LDA",bx,-1-(var->offset),bp,"get local address");
        }
        break;
    case PARAM:
        if(var->type == TYPE_ARRAY) {
            generateRegMem("LD",bx,2+(var->offset),bp,"get param array address");
        } else {
            generateRegMem("LDA",bx,2+(var->offset),bp,"get param variable address");
        }
        break;
    }
}


void generateFunCall(FunSymbol *fun) {

    generateRegMem("LDA",ax,3,pc,"store returned PC");
    generateRegMem("LDA",sp,-1,sp,"push prepare");
    generateRegMem("ST",ax,0,sp,"push returned PC");
    generateRegMem("LDC",pc,fun->offset,0,"jump to function");
    generateRegMem("LDA",sp,fun->paramNum,sp,"release parameters");
}


static int getValue = 1;
static int isRecursive = 1;


void recursiveGen(TreeNode *tree) {
    int tmp;
    TreeNode *p1, *p2, *p3;
    int savedLoc1,savedLoc2,currentLoc;
    VarSymbol *var;
    FunSymbol *fun;

    while(tree) {
        switch (tree->astType) {
        case FUNDEC_AST:
            if (TraceCode)
                generateComment("-> function:");
            p1 = tree->child[0];
            p2 = tree->child[1];
            fun = getFunction(p1->attr.name);
            fun->offset = generateSkip(0);
            generateRegMem("LDA",sp,-1,sp,"push prepare");
            generateRegMem("ST",bp,0,sp,"push old bp");
            generateRegMem("LDA",bp,0,sp,"let bp == sp");
            generateRegMem("LDA",sp,-(p2->symbolTable->size),sp,"allocate for local variables");
            pushTable(fun->symbolTable);
            recursiveGen(p2);
            popTable();
            if(p1->type == TYPE_VOID) {
                generateRegMem("LDA",sp,0,bp,"let sp == bp");
                generateRegMem("LDA",sp,2,sp,"pop prepare");
                generateRegMem("LD",bp,-2,sp,"pop old bp");
                generateRegMem("LD",pc,-1,sp,"pop return addr");
            }
            if (TraceCode)
                generateComment("<- function");
            break;

        case COMPOUND_AST:
            if (TraceCode)
                generateComment("-> compound");
            p1 = tree->child[1];
            if(tree->symbolTable)
                pushTable(tree->symbolTable);
            recursiveGen(p1);
            if(tree->symbolTable)
                popTable();
            if (TraceCode)
                generateComment("<- compound");
            break;

        case SELESTMT_AST:
            if (TraceCode)
                generateComment("-> if");
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];
            recursiveGen(p1);
            savedLoc1 = generateSkip(1);
            generateComment("jump to else ");
            recursiveGen(p2);
            savedLoc2 = generateSkip(1);
            generateComment("jump to end");
            currentLoc = generateSkip(0);
            generateRewind(savedLoc1);
            generateRegMem("JEQ",ax,currentLoc,zero,"if: jmp to else");
            generateRestore();
            recursiveGen(p3);
            currentLoc = generateSkip(0);
            generateRewind(savedLoc2);
            generateRegMem("LDA",pc,currentLoc,zero,"jmp to end");
            generateRestore();
            if (TraceCode)
                generateComment("<- if");
            break;

        case ITERSTMT_AST:
            if (TraceCode)
                generateComment("-> while");
            p1 = tree->child[0];
            p2 = tree->child[1];
            savedLoc1 = generateSkip(0);
            generateComment("jump here after body");
            recursiveGen(p1);
            savedLoc2 = generateSkip(1);
            generateComment("jump to end if test fails");
            recursiveGen(p2);
            generateRegMem("LDA",pc,savedLoc1,zero,"jump to test");
            currentLoc = generateSkip(0);
            generateRewind(savedLoc2);
            generateRegMem("JEQ",ax,currentLoc,zero,"jump to end");
            generateRestore();
            if (TraceCode)
                generateComment("<- while");
            break;

        case RETSTMT_AST:
            if (TraceCode)
                generateComment("-> return");
            p1 = tree->child[0];
            if(tree->type != TYPE_VOID)
                recursiveGen(p1);
            generateRegMem("LDA",sp,0,bp,"let sp == bp");
            generateRegMem("LDA",sp,2,sp,"pop prepare");
            generateRegMem("LD",bp,-2,sp,"pop old bp");
            generateRegMem("LD",pc,-1,sp,"pop return addr");
            if (TraceCode)
                generateComment("<- return");
            break;

        case NUM_AST:
            if(TraceCode)
                generateComment("-> number");
            generateRegMem("LDC",ax,tree->attr.value,0,"store number");
            if(TraceCode)
                generateComment("<- number");
            break;

        case VAR_AST:
            if(TraceCode)
                generateComment("-> variable");
            var = getVariable(tree->attr.name);
            generateGetAddr(var);
            if(getValue) {
                if(var->type == TYPE_ARRAY) {
                    generateRegMem("LDA",ax,0,bx,"get array variable value( == address)");
                } else {
                    generateRegMem("LD",ax,0,bx,"get variable value");
                }
            }
            if(TraceCode)
                generateComment("<- variable");
            break;

        case ARRAYVAR_AST:
            if(TraceCode)
                generateComment("-> array element");
            p1 = tree->child[0];
            var = getVariable(tree->attr.name);
            generateGetAddr(var);
            generateRegMem("LDA",sp,-1,sp,"push prepare");
            generateRegMem("ST",bx,0,sp,"protect array address");
            tmp = getValue;
            getValue = 1;
            recursiveGen(p1);
            getValue = tmp;
            generateRegMem("LDA",sp,1,sp,"pop prepare");
            generateRegMem("LD",bx,-1,sp,"recover array address");
            generateRegOnly("SUB",bx,bx,ax,"get address of array element");
            if(getValue)
                generateRegMem("LD",ax,0,bx,"get value of array element");
            if(TraceCode)
                generateComment("<- array element");
            break;

        case ASSIGN_AST:
            if (TraceCode)
                generateComment("-> assign");
            p1 = tree->child[0];
            p2 = tree->child[1];
            getValue = 0;
            recursiveGen(p1);
            generateRegMem("LDA",sp,-1,sp,"push prepare");
            generateRegMem("ST",bx,0,sp,"protect bx");
            getValue = 1;
            recursiveGen(p2);
            generateRegMem("LDA",sp,1,sp,"pop prepare");
            generateRegMem("LD",bx,-1,sp,"recover bx");
            generateRegMem("ST",ax,0,bx,"assign: store");
            if (TraceCode)
                generateComment("<- assign");
            break;

        case EXP_AST:
            if (TraceCode)
                generateComment("-> op");
            p1 = tree->child[0];
            p2 = tree->child[1];
            recursiveGen(p1);
            generateRegMem("LDA",sp,-1,sp,"push prepare");
            generateRegMem("ST",ax,0,sp,"op: protect left");
            recursiveGen(p2);
            generateRegMem("LDA",sp,1,sp,"pop prepare");
            generateRegMem("LD",bx,-1,sp,"op: recover left");
            switch (tree->attr.op) {
            case PLUS :
                generateRegOnly("ADD",ax,bx,ax,"op +");
                break;
            case MINUS :
                generateRegOnly("SUB",ax,bx,ax,"op -");
                break;
            case MULTI :
                generateRegOnly("MUL",ax,bx,ax,"op *");
                break;
            case DIV :
                generateRegOnly("DIV",ax,bx,ax,"op /");
                break;
            case EQ :
                generateRegOnly("SUB",ax,bx,ax,"op ==");
                generateRegMem("JEQ",ax,2,pc,"br if true");
                generateRegMem("LDC",ax,0,0,"false case");
                generateRegMem("LDA",pc,1,pc,"unconditional jmp");
                generateRegMem("LDC",ax,1,0,"true case");
                break;
            case NE :
                generateRegOnly("SUB",ax,bx,ax,"op !=");
                generateRegMem("JNE",ax,2,pc,"br if true");
                generateRegMem("LDC",ax,0,0,"false case");
                generateRegMem("LDA",pc,1,pc,"unconditional jmp");
                generateRegMem("LDC",ax,1,0,"true case");
                break;
            case LT :
                generateRegOnly("SUB",ax,bx,ax,"op <");
                generateRegMem("JLT",ax,2,pc,"br if true");
                generateRegMem("LDC",ax,0,0,"false case");
                generateRegMem("LDA",pc,1,pc,"unconditional jmp");
                generateRegMem("LDC",ax,1,0,"true case");
                break;
            case GT :
                generateRegOnly("SUB",ax,bx,ax,"op >");
                generateRegMem("JGT",ax,2,pc,"br if true");
                generateRegMem("LDC",ax,0,0,"false case");
                generateRegMem("LDA",pc,1,pc,"unconditional jmp");
                generateRegMem("LDC",ax,1,0,"true case");
                break;
            case LE :
                generateRegOnly("SUB",ax,bx,ax,"op <=");
                generateRegMem("JLE",ax,2,pc,"br if true");
                generateRegMem("LDC",ax,0,0,"false case");
                generateRegMem("LDA",pc,1,pc,"unconditional jmp");
                generateRegMem("LDC",ax,1,0,"true case");
                break;
            case GE :
                generateRegOnly("SUB",ax,bx,ax,"op >=");
                generateRegMem("JGE",ax,2,pc,"br if true");
                generateRegMem("LDC",ax,0,0,"false case");
                generateRegMem("LDA",pc,1,pc,"unconditional jmp");
                generateRegMem("LDC",ax,1,0,"true case");
                break;
            default:
                generateComment("BUG: Unknown operator");
                break;
            }
            if (TraceCode)
                generateComment("<- op");
            break;

        case CALL_AST:
            if (TraceCode)
                generateComment("-> call");
            p1 = tree->child[0];
            while(p1 != NULL) {
                pushParam(p1);
                p1 = p1->sibling;
            }
            isRecursive = 0;
            while((p1 = popParam()) != NULL) {
                recursiveGen(p1);
                generateRegMem("LDA",sp,-1,sp,"push prepare");
                generateRegMem("ST",ax,0,sp,"push parameters");
            }
            isRecursive = 1;
            fun = getFunction(tree->attr.name);
            generateFunCall(fun);
            if (TraceCode)
                generateComment("<- call");

            break;
            default:
            break;
        }

        if(isRecursive) {
            tree = tree->sibling;
        } else {
            break;
        }
    }
}


void generateCode() {

    generatePrelude();
    if (TraceCode)
        generateComment("Jump to main()");
    int loc = generateSkip(6);
    generateInput();
    generateOutput();
    recursiveGen(ASTRoot);
    generateRewind(loc);
    FunSymbol *fun = getFunction("main");
    generateFunCall(fun);
    generateRegOnly("HALT",0,0,0,"END OF PROGRAM");
}
