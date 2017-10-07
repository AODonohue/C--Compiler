CC = gcc
LDFLAGS = -ll -ly

LEX = flex
YACC = bison
YFLAGS = -d

SRC = main.c scan.c parse.c SyntaxTree.c SymbolTable.c CodeGeneration.c


all: cm

cm: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ -g

scan.c: scan.l globals.h parse.h
	$(LEX) $(LFLAGS) -o $@ $< 

parse.c parse.h: parse.y globals.h SyntaxTree.h
	$(YACC) $(YFLAGS) -o $@ $< 

clean:
	rm -f *.o 
	rm -f scan.c
	rm -f parse.c
	rm -f parse.h
