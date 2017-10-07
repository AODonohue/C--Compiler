/*********************************************************************
 * FILE NAME: main.c
 * AUTHOR: Andrew O'Donohue
 * PURPOSE: Main terminal interface.
 *********************************************************************/
#include "globals.h"
#include "parse.h"
#include "SymbolTable.h"
#include "CodeGeneration.h"
#include "SyntaxTree.h"

#define FILE_NAME_LEN 100

int AST = FALSE;
int Table = FALSE;
int Assembly = FALSE;
FILE *source;
FILE *listing;
FILE *code;

int main(int argc, char *argv[]) {

    char sourcefile[FILE_NAME_LEN];

    if (argc < 2) {
		fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
    	exit(1);
    }
    if (argc == 3) {
    	if(strcmp(argv[2], "-a") == 0)
    		AST = TRUE;
    	if(strcmp(argv[2], "-s") == 0)
    		Table = TRUE;
    	if(strcmp(argv[2], "-c") == 0)
    		Assembly = TRUE;
    }
    if (argc == 4) {
    	if(strcmp(argv[2], "-a") == 0 || strcmp(argv[3], "-a") == 0)
    		AST = TRUE;
    	if(strcmp(argv[2], "-s") == 0 || strcmp(argv[3], "-s") == 0)
    		Table = TRUE;
    	if(strcmp(argv[2], "-c") == 0 || strcmp(argv[3], "-c") == 0)
    		Assembly = TRUE;
    }
    if (argc == 5) {
    	if(strcmp(argv[2], "-a") == 0 || strcmp(argv[3], "-a") == 0 || strcmp(argv[4], "-a") == 0)
    		AST = TRUE;
    	if(strcmp(argv[2], "-s") == 0 || strcmp(argv[3], "-s") == 0 || strcmp(argv[4], "-s") == 0)
    		Table = TRUE;
    	if(strcmp(argv[2], "-c") == 0 || strcmp(argv[3], "-c") == 0  || strcmp(argv[4], "-c") == 0)
    		Assembly = TRUE;
    }

    strcpy(sourcefile,argv[1]) ;
    source = fopen(sourcefile,"r");
    ASSERT(source != NULL) {
        fprintf(stderr,"File %s not found.\n",sourcefile);
    }

    listing = stdout;
    fprintf(listing,"\nC minus compilation: %s\n",sourcefile);
    initTable();
    yyrestart(source);
    yyparse();
    fclose(source);

    if (AST == TRUE)
    	printAST(ASTRoot,0);

    if (Assembly) {
    	char *codefile = (char *) calloc(strlen(sourcefile), sizeof(char));
    	strcpy(codefile,sourcefile);
    	strcat(codefile,".tm");
    	code = fopen(codefile,"w");
    	ASSERT(code != NULL) {
    		fprintf(stderr, "Unable to open %s for output.\n",codefile);
    	}
    	generateCode();
    	fclose(code);
    }

    return 0;
}
