/*********************************************************************
 * FILE NAME: SyntaxTree.c
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: Functions to create and manipulate syntax tree.
 *********************************************************************/
#include "globals.h"
#include "parse.h"
#include "SymbolTable.h"
#include "SyntaxTree.h"

TreeNode *ASTRoot;

static Scope current_scope = GLOBAL;
static FunSymbol *current_fun = NULL;


TreeNode *newDecList(TreeNode* decList, TreeNode* declaration) {
    TreeNode* node = decList;

    while(node->sibling != NULL) {
        node = node->sibling;
    }
    node->sibling = declaration;
    return decList;
}

TreeNode *newTypeSpe(ExpType type, int lineno) {
    TreeNode *root = newASTNode(TYPE_AST, lineno);

    root->type = type;
    return root;
}


TreeNode *newVarDec(TreeNode *typeSpecifier, char *ID, int lineno) {

    ASSERT(typeSpecifier->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, type specifier of variable %s must be int.\n", lineno, ID);
    }
    if(current_scope == LOCAL)
        pushTable(CompoundST);
    ASSERT(getTopVar(ID) == NULL) {
        fprintf(stderr, "Error: @line %d, duplicate declarations of variable %s.\n", lineno, ID);
    }

    TreeNode *root = newASTNode(VARDEC_AST, lineno);
    root->child[0] = typeSpecifier;
    root->attr.name = strdup(ID);
    root->type = TYPE_INTEGER;

    if(current_scope == LOCAL) {
        putVariable(root->attr.name, LOCAL, tables->size++, TYPE_INTEGER);
        popTable();
    } else {
        putVariable(root->attr.name, GLOBAL, tables->size++, TYPE_INTEGER);
    }
    return root;
}


TreeNode *newArrayDec(TreeNode *typeSpecifier, char *ID, int size, int lineno) {

    ASSERT(typeSpecifier->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, type specifier of variable %s must be int.\n", lineno, ID);
    }
    if(current_scope == LOCAL)
        pushTable(CompoundST);
    ASSERT(getTopVar(ID) == NULL) {
        fprintf(stderr, "Error: @line %d, duplicate declarations of variable %s.\n", lineno, ID);
    }

    TreeNode *root = newASTNode(ARRAYDEC_AST, lineno);
    root->child[0] = typeSpecifier;
    root->attr.name = strdup(ID);
    root->type = TYPE_ARRAY;
    root->attr.value = size;


    if(current_scope == LOCAL) {
        putVariable(root->attr.name, LOCAL, tables->size, TYPE_ARRAY);
        tables->size += size;
        popTable();
    } else {
        putVariable(root->attr.name, GLOBAL, tables->size, TYPE_ARRAY);
        tables->size += size;
    }
    return root;
}


TreeNode *newFunDec(TreeNode *funHead, TreeNode *funBody, int lineno) {

    TreeNode *root = newASTNode(FUNDEC_AST, lineno);
    root->child[0] = funHead;
    root->child[1] = funBody;
    funBody->symbolTable = CompoundST;
    CompoundST = newSymbolTable(LOCAL);
    popTable();
    current_scope = GLOBAL;
    current_fun = NULL;
    return root;
}


TreeNode *newFunHead(TreeNode *typeSpecifier, char *ID, TreeNode *params, int lineno) {

    ASSERT(getFunction(ID) == NULL) {
        fprintf(stderr, "Error: @line %d, duplicate declarations of function %s.\n", lineno, ID);
    }

    TreeNode *root = newASTNode(FUNHEAD_AST, lineno);
    root->attr.name = strdup(ID);
    root->type = typeSpecifier->type;
    root->child[0] = typeSpecifier;
    root->child[1] = params;

    if (Table)
    	fprintf(listing, "Symbol table of function: %s\n", root->attr.name);

    putFunction(ID, ParamST, ParamST->size, root->type);
    pushTable(ParamST);
    current_scope = LOCAL;
    current_fun = getFunction(ID);
    ParamST = newSymbolTable(PARAM);
    return root;
}


TreeNode *newParamList(TreeNode *paramList, TreeNode *param) {

    TreeNode *node = paramList;
    if(paramList != NULL) {
        while(node->sibling != NULL) {
            node = node->sibling;
        }
        node->sibling = param;
        return paramList;
    } else {
        return param;
    }
}


TreeNode *newParam(TreeNode *typeSpecifier, char *ID, int isArray, int lineno) {

    ASSERT(typeSpecifier->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, type specifier of param %s must be int.\n", lineno, ID);
    }
    pushTable(ParamST);
    ASSERT(getTopVar(ID) == NULL) {
        fprintf(stderr, "Error: @line %d, duplicate declarations of variable %s.\n", lineno, ID);
    }

    TreeNode *root;
    if(!isArray) {
        root = newASTNode(PARAMID_AST, lineno);
        root->child[0] = typeSpecifier;
        root->attr.name = strdup(ID);
        root->type = TYPE_INTEGER;
        putVariable(root->attr.name, PARAM, ParamST->size++ , TYPE_INTEGER);
    } else {
        root = newASTNode(PARAMARRAY_AST, lineno);
        root->child[0] = typeSpecifier;
        root->attr.name = strdup(ID);
        root->type = TYPE_ARRAY;
        putVariable(root->attr.name, PARAM, ParamST->size++ , TYPE_ARRAY);
    }
    popTable();
    return root;
}


TreeNode *newCompound(TreeNode *localDecs, TreeNode *stmtList, int lineno) {

    TreeNode *root = newASTNode(COMPOUND_AST, lineno);
    root->child[0] = localDecs;
    root->child[1] = stmtList;

    if(stmtList != NULL && stmtList->type != TYPE_UNDEFINED) {
        root->type = stmtList->type;
    } else {
        root->type = TYPE_VOID;
    }
    return root;
}


TreeNode *newLocalDecs(TreeNode *localDecs, TreeNode *varDec) {

    if(localDecs == NULL)
        return varDec;
    TreeNode *node = localDecs;
    while(node->sibling != NULL) {
        node = node->sibling;
    }
    node->sibling = varDec;

    return localDecs;
}


TreeNode *newStmtList(TreeNode *stmtList, TreeNode *stmt, int lineno) {

    if(stmtList == NULL)
        return stmt;
    TreeNode *node = stmtList;
    while(node->sibling != NULL) {
        node = node->sibling;
    }
    node->sibling = stmt;

    return stmtList;
}


TreeNode *newSelectStmt(TreeNode *expression, TreeNode *stmt, TreeNode *elseStmt, int lineno) {

    ASSERT(expression->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, test condition expression not integer.\n", lineno);
    }
    TreeNode *root = newASTNode(SELESTMT_AST, lineno);
    root->child[0] = expression;
    root->child[1] = stmt;
    root->child[2] = elseStmt;

    return root;
}


TreeNode *newIterStmt(TreeNode *expression,  TreeNode *stmt, int lineno) {

    ASSERT(expression->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, test condition expression not integer.\n", lineno);
    }
    TreeNode *root = newASTNode(ITERSTMT_AST, lineno);
    root->child[0] = expression;
    root->child[1] = stmt;

    return root;
}


TreeNode *newRetStmt(TreeNode *expression, int lineno) {
    ExpType type;

    if(expression!= NULL) {
        type = expression->type;
    } else {
        type = TYPE_VOID;
    }
    ASSERT(type == current_fun->type) {
        fprintf(stderr, "Error: @line %d, return type mis-match.\n", lineno);
    }
    TreeNode *root = newASTNode(RETSTMT_AST, lineno);
    root->child[0] = expression;
    root->type = type;
    if (Table)
    	printSymTab(CompoundST);

    return root;
}


TreeNode *newAssignExp(TreeNode *var, TreeNode *expression, int lineno) {

    ASSERT(var->type==TYPE_INTEGER && expression->type==TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, only can assign int to int.\n", lineno);
    }
    TreeNode *root = newASTNode(ASSIGN_AST, lineno);
    root->child[0] = var;
    root->child[1] = expression;
    root->type = TYPE_INTEGER;

    return root;
}


TreeNode *newVar(char *ID, int lineno) {

    pushTable(CompoundST);
    VarSymbol *vs = getVariable(ID);
    ASSERT(vs != NULL) {
        fprintf(stderr, "Error: @line %d, variable %s not defined before.\n", lineno, ID);
    }
    popTable();
    TreeNode *root = newASTNode(VAR_AST, lineno);
    root->attr.name = vs->name;
    root->type = vs->type;

    return root;
}


TreeNode *newArrayVar(char *ID, TreeNode *expression, int lineno) {

    ASSERT(expression->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, array %s: index is not integer.\n", lineno, ID);

    }
    pushTable(CompoundST);
    VarSymbol *vs = getVariable(ID);
    ASSERT(vs != NULL) {
        fprintf(stderr, "Error: @line %d, variable %s not defined before.\n", lineno, ID);
    }
    popTable();
    ASSERT(vs->type == TYPE_ARRAY) {
        fprintf(stderr, "Error: @line %d, variable %s is not an array.\n", lineno, ID);
    }
    TreeNode *root = newASTNode(ARRAYVAR_AST, lineno);
    root->child[0] = expression;
    root->attr.name = vs->name;
    root->type = TYPE_INTEGER;

    return root;
}


TreeNode *newSimpExp(TreeNode *addExp1, int relop, TreeNode *addExp2, int lineno) {

    ASSERT(addExp1->type == TYPE_INTEGER && addExp2->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, only can compare integers.\n", lineno);
    }
    TreeNode *root = newASTNode(EXP_AST, lineno);
    root->child[0] = addExp1;
    root->child[1] = addExp2;
    root->attr.op = relop;
    root->type = TYPE_INTEGER;

    return root;
}


TreeNode *newAddExp(TreeNode *addExp, int addop, TreeNode *term, int lineno) {

    ASSERT(addExp->type == TYPE_INTEGER && term->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, only can calculate integers.\n", lineno);
    }
    TreeNode *root = newASTNode(EXP_AST, lineno);
    root->child[0] = addExp;
    root->child[1] = term;
    root->attr.op = addop;
    root->type = TYPE_INTEGER;

    return root;
}


TreeNode *newTerm(TreeNode *term, int mulop, TreeNode *factor, int lineno) {

    ASSERT(term->type == TYPE_INTEGER && factor->type == TYPE_INTEGER) {
        fprintf(stderr, "Error: @line %d, only can calculate integers.\n", lineno);

    }
    TreeNode *root = newASTNode(EXP_AST, lineno);
    root->child[0] = term;
    root->child[1] = factor;
    root->attr.op = mulop;
    root->type = TYPE_INTEGER;

    return root;
}


TreeNode *newNumNode(int value, int lineno) {

    TreeNode *root = newASTNode(NUM_AST, lineno);
    root->attr.value = value;
    root->type = TYPE_INTEGER;

    return root;
}


TreeNode *newCall(char *ID, TreeNode *args, int lineno) {

    FunSymbol *fun = getFunction(ID);
    ASSERT(fun != NULL) {
        fprintf(stderr, "Error: @line %d, call function %s which is not defined.\n", lineno, ID);
    }
    VarSymbol *var = fun->symbolTable->varList;
    TreeNode *tmp = args;
    while(var && tmp) {
        ASSERT(var->type == tmp->type) {
            fprintf(stderr, "Error: @line %d, call function %s : parameter type mis-match.\n", lineno, ID);
        }
        var = var->next_FIFO;
        tmp = tmp->sibling;
    }
    ASSERT(!var && !tmp) {
        fprintf(stderr, "Error: @line %d, call function %s : parameter number mis-match.\n", lineno, ID);
    }
    TreeNode *root = newASTNode(CALL_AST, lineno);
    root->child[0] = args;
    root->attr.name = strdup(ID);
    root->type = fun->type;

    return root;
}


TreeNode *newArgList(TreeNode *argList, TreeNode *expression) {

    TreeNode *node = argList;
    while(node->sibling != NULL) {
        node = node->sibling;
    }
    node->sibling = expression;

    return argList;
}


TreeNode *newASTNode(ASTType type, int lineno) {
	int i;

    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    ASSERT(node != NULL) {
        fprintf(stderr, "Failed to malloc for TreeNode @line%d.\n", lineno);
    }
    for(i=0; i<MAXCHILDREN; ++i) {
        node->child[i] = NULL;
    }
    node->sibling = NULL;
    node->astType = type;
    node->type = TYPE_UNDEFINED;
    node->lineno = lineno;

    return node;
}


void printNodeKind(TreeNode *node) {
    if(node == NULL)
        return;

    switch(node->astType) {
    case VARDEC_AST:
        printf("Var declaration \n");
        break;
    case ARRAYDEC_AST:
        printf("Array declaration \n");
        break;
    case FUNDEC_AST:
        printf("Function declaration \n");
        break;
    case TYPE_AST:
        printf("Type specifier \n");
        break;
    case PARAMID_AST:
        printf("Param of ID \n");
        break;
    case PARAMARRAY_AST:
        printf("Param of Array \n");
        break;
    case COMPOUND_AST:
        printf("Counpound statements \n");
        break;
    case EXPSTMT_AST:
        printf("Expression statement\n");
        break;
    case SELESTMT_AST:
        printf("Select statement\n");
        break;
    case ITERSTMT_AST:
        printf("Iteration statement\n");
        break;
    case RETSTMT_AST:
        printf("Return statement\n");
        break;
    case ASSIGN_AST:
        printf("Assign statement\n");
        break;
    case EXP_AST:
        printf("Expression \n");
        break;
    case VAR_AST:
        printf("Var \n");
        break;
    case ARRAYVAR_AST:
        printf("Array var ASt\n");
        break;
    case FACTOR_AST:
        printf("Factor \n");
        break;
    case CALL_AST:
        printf("Call stement \n");
        break;
    case NUM_AST:
        printf("Number \n");
        break;
    default:
    	break;
    }
}


void printAST(TreeNode *root, int indent) {
	int i;

    TreeNode *node = root;
    while(node != NULL) {
        for (i = 0; i<indent; ++i) {
            printf("  ");
        }
        printNodeKind(node);
        printAST(node->child[0], indent+4);
        printAST(node->child[1], indent+4);
        printAST(node->child[2], indent+4);
        printAST(node->child[3], indent+4);
        node = node->sibling;
    }
}
