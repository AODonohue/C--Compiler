/*********************************************************************
 * FILE NAME: SyntaxTree.h
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: SyntaxTree.c public interface.
 *********************************************************************/
#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include "globals.h"

extern TreeNode *ASTRoot;


/*********************************************************************
 * FUNCTION NAME: newASTNode
 * PURPOSE: Adds a new node to a syntax tree
 * ARGUMENTS: . The type of node to add (ASTType) 
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newASTNode(ASTType asttype, int lineno);


/*********************************************************************
 * FUNCTION NAME: newDecList
 * PURPOSE: Adds a new declaration list to a syntax tree
 * ARGUMENTS: . The node to add the list to (TreeNode *) 
 *            . The declaration list to add (TreeNode *)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newDecList(TreeNode *decList, TreeNode *declaration);


/*********************************************************************
 * FUNCTION NAME: newDec
 * PURPOSE: Adds a new declaration to a syntax tree
 * ARGUMENTS: . The node to add the list to (TreeNode *) 
 *            . The declaration to add (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newDec(TreeNode *declaration, int type);


/*********************************************************************
 * FUNCTION NAME: newVarDec
 * PURPOSE: Adds a new variable declaration to a syntax tree
 * ARGUMENTS: . The type of variable to add (TreeNode *) 
 *            . The ID of the variable (char *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newVarDec(TreeNode *typeSpecifier, char *ID, int lineno);


/*********************************************************************
 * FUNCTION NAME: newArrayDec
 * PURPOSE: Adds a new array declaration to a syntax tree
 * ARGUMENTS: . The type of array to add (TreeNode *) 
 *            . The ID of the array (char *)
 *            . The size of the array (int)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newArrayDec(TreeNode *typeSpecifier, char *ID, int size, int lineno);


/*********************************************************************
 * FUNCTION NAME: newTypeSpe
 * PURPOSE: Adds exp type to a syntax tree
 * ARGUMENTS: . The type to add (ExpType)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newTypeSpe(ExpType type, int lineno);


/*********************************************************************
 * FUNCTION NAME: newFunDec
 * PURPOSE: Adds a new function declaration to a syntax tree
 * ARGUMENTS: . The function head to add (TreeNode *) 
 *            . The function body to add (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newFunDec(TreeNode *funHead, TreeNode *funBody, int lineno);


/*********************************************************************
 * FUNCTION NAME: newFunHead
 * PURPOSE: Adds a function head to a syntax tree
 * ARGUMENTS: . The type of function to add (TreeNode *) 
 *            . The ID of the function (char *)
 *            . The parameters of the function (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newFunHead(TreeNode *typeSpecifier, char *ID, TreeNode *params, int lineno);


/*********************************************************************
 * FUNCTION NAME: newParamList
 * PURPOSE: Adds a new parameter list to a syntax tree
 * ARGUMENTS: . The parameter list to add (TreeNode *) 
 *            . The initial parameter in the list (TreeNode *)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newParamList(TreeNode *paramList, TreeNode *param);


/*********************************************************************
 * FUNCTION NAME: newParam
 * PURPOSE: Adds a new parameter to a syntax tree
 * ARGUMENTS: . The type of parameter to add (TreeNode *) 
 *            . The ID of the parameter (char *)
 *            . The type of parameter (int)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newParam(TreeNode *typeSpecifier, char *ID, int type, int lineno);


/*********************************************************************
 * FUNCTION NAME: newCompound
 * PURPOSE: Adds a new compound to a syntax tree
 * ARGUMENTS: . The local declarations to add (TreeNode *) 
 *            . The statement list to add (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newCompound(TreeNode *localDecs, TreeNode *stmtList, int lineno);


/*********************************************************************
 * FUNCTION NAME: newLocalDecs
 * PURPOSE: Adds a new local declaration to a syntax tree
 * ARGUMENTS: . The local declarations to add (TreeNode *) 
 *            . The variable declarations to add (TreeNode *)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newLocalDecs(TreeNode *localDecs, TreeNode *varDec);


/*********************************************************************
 * FUNCTION NAME: newStmtList
 * PURPOSE: Adds a new statement list to a syntax tree
 * ARGUMENTS: . The statement list to add (TreeNode *) 
 *            . The initial statement in the list (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newStmtList(TreeNode *stmtList, TreeNode *stmt, int lineno);


/*********************************************************************
 * FUNCTION NAME: newExpStmt
 * PURPOSE: Adds a new expression statement to a syntax tree
 * ARGUMENTS: . The expression to add (TreeNode *) 
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newExpStmt(TreeNode *expression,int lineno);


/*********************************************************************
 * FUNCTION NAME: newSelectStmt
 * PURPOSE: Adds a new select statement to a syntax tree
 * ARGUMENTS: . The expression to add (TreeNode *) 
 *            . The statement to add (TreeNode *)
 *            . The else statement to add (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newSelectStmt(TreeNode *expression, TreeNode *stmt, TreeNode *elseStmt, int lineno);


/*********************************************************************
 * FUNCTION NAME: newIterStmt
 * PURPOSE: Adds a new iteration statement to a syntax tree
 * ARGUMENTS: . The expression to add (TreeNode *) 
 *            . The statement to add (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newIterStmt(TreeNode *expression,  TreeNode *stmt, int lineno);


/*********************************************************************
 * FUNCTION NAME: newRetStmt
 * PURPOSE: Adds a new return statement to a syntax tree
 * ARGUMENTS: . The expression to add (TreeNode *) 
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newRetStmt(TreeNode *expression, int lineno);


/*********************************************************************
 * FUNCTION NAME: newAssignExp
 * PURPOSE: Adds a new assignment expression to a syntax tree
 * ARGUMENTS: . The variable to be assigned (TreeNode *) 
 *            . The the expression to add (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newAssignExp(TreeNode *var, TreeNode *expression, int lineno);


/*********************************************************************
 * FUNCTION NAME: newVar
 * PURPOSE: Creates a new variable
 * ARGUMENTS: . The ID of the variable (char *) 
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newVar(char *ID, int lineno);


/*********************************************************************
 * FUNCTION NAME: newArrayVar
 * PURPOSE: Creates a new array variable
 * ARGUMENTS: . The ID of the array (char *) 
 *            . The expression of the array (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newArrayVar(char *ID, TreeNode *expression, int lineno);


/*********************************************************************
 * FUNCTION NAME: newSimpExp
 * PURPOSE: Adds a new subtract expression
 * ARGUMENTS: . The first variable to subtract (TreeNode *) 
 *            . The subtraction of the numbers (int)
 *            . The second variable to subtract (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newSimpExp(TreeNode *addExp1, int relop, TreeNode *addExp2, int lineno);


/*********************************************************************
 * FUNCTION NAME: newAddExp
 * PURPOSE: Adds a new addition expression
 * ARGUMENTS: . The first variable to add (TreeNode *) 
 *            . The addition of the numbers (int)
 *            . The second variable to add (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newAddExp(TreeNode *addExp, int addop, TreeNode *term, int lineno);


/*********************************************************************
 * FUNCTION NAME: newTerm
 * PURPOSE: Adds a new multiplication expression
 * ARGUMENTS: . The first variable to multiply (TreeNode *) 
 *            . The multiplication of the numbers (int)
 *            . The second variable to multiply (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newTerm(TreeNode *term, int mulop, TreeNode *factor, int lineno);


/*********************************************************************
 * FUNCTION NAME: newNumNode
 * PURPOSE: Adds a new number to a syntax tree
 * ARGUMENTS: . The number to add (int) 
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newNumNode(int num, int lineno);


/*********************************************************************
 * FUNCTION NAME: newCall
 * PURPOSE: Adds a new function call to a syntax tree
 * ARGUMENTS: . The name of the function to call (char *) 
 *            . The arguments of the function (TreeNode *)
 *            . The initial line number in code (int)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newCall(char *ID, TreeNode *args, int lineno);


/*********************************************************************
 * FUNCTION NAME: newArgList
 * PURPOSE: Adds a new argument list to a syntax tree
 * ARGUMENTS: . The argument list to add (TreeNode *) 
 *            . The expression to add (TreeNode *)
 * RETURNS: A pointer to the syntax tree
 *********************************************************************/
TreeNode *newArgList(TreeNode *argList, TreeNode *expression);


/*********************************************************************
 * FUNCTION NAME: printAST
 * PURPOSE: Prints a syntax tree to stdout
 * ARGUMENTS: . The root of the tree to print (TreeNode *) 
 *            . The size of the indents in number of spaces (int)
 *********************************************************************/
void printAST(TreeNode *root, int indent);

#endif
