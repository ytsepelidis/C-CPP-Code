all: compiler

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

parser.tab.c parser.tab.h: parser.y
	bison -v -d parser.y

compiler: parser.tab.c parser.tab.h lex.yy.c hash/hashtbl.c hash/hashtbl.h
	gcc parser.tab.c lex.yy.c hash/hashtbl.c -o pasc -Wall -lm

clean:
	rm -f pasc lex.yy.c parser.tab.c parser.tab.h parser.output
