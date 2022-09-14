%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <math.h>
	#include <string.h>
	#include "hash/hashtbl.h"

	int errorCounter = 0;
	HASHTBL *symbolTable;
	int scope = 0;
	
	void yyerror(const char* err);
	extern int yylineno; 
	extern int yylex();
	extern char str_buf[256];
	extern char *yytext;
	extern FILE *yyin;
%}

%define parse.error verbose

%union
{
	int intval;
	double realval;
	char charval;
	char* strval;
}

%token <strval> T_PROGRAM		"program"
%token <strval> T_CONST		"const"
%token <strval> T_TYPE			"type"
%token <strval> T_ARRAY		"array"
%token <strval> T_OF			"of"
%token <strval> T_VAR			"var"
%token <strval> T_FORWARD		"forward"
%token <strval> T_FUNCTION		"function"
%token <strval> T_PROCEDURE		"procedure"
%token <strval> T_INTEGER		"integer"
%token <strval> T_REAL			"real"
%token <strval> T_BOOLEAN		"boolean"
%token <strval> T_CHAR			"char"
%token <strval> T_STRING		"string"
%token <strval> T_BEGIN		"begin"
%token <strval> T_END			"end"
%token <strval> T_IF			"if"
%token <strval> T_THEN			"then"
%token <strval> T_ELSE			"else"
%token <strval> T_CASE			"case"
%token <strval> T_OTHERWISE		"otherwise"
%token <strval> T_WHILE		"while"
%token <strval> T_DO			"do"
%token <strval> T_FOR			"for"
%token <strval> T_DOWNTO		"downto"
%token <strval> T_TO			"to"
%token <strval> T_READ			"read"
%token <strval> T_WRITE		"write"
%token <strval> T_LENGTH		"length"

%token <strval>  T_ID			"id"
%token <intval>  T_ICONST		"integer const"
%token <realval> T_RCONST		"real const"
%token <strval>  T_BCONST		"boolean const"
%token <charval> T_CCONST		"character const"
%token <strval>  T_SCONST		"string const"

%token <strval> T_RELOP		"> or >= or < or <= or <>"
%token <strval> T_ADDOP		"+ or -"
%token <strval> T_OROP			"OR"
%token <strval> T_MULDIVANDOP		"* or / or DIV or MOD or AND"
%token <strval> T_NOTOP		"NOT"

%token <strval> T_LPAREN		"("
%token <strval> T_RPAREN		")"
%token <strval> T_SEMI			";"
%token <strval> T_DOT			"."
%token <strval> T_COMMA		","
%token <strval> T_EQU			"="
%token <strval> T_COLON		":"
%token <strval> T_LBRACK		"["
%token <strval> T_RBRACK		"]"
%token <strval> T_ASSIGN		".="
%token <strval> T_DOTDOT		".."

%token T_EOF			0	"end of file"

%right T_ASSIGN
%left T_RELOP T_EQU
%left T_ADDOP T_OROP
%left T_MULDIVANDOP
%left T_NOTOP
%left T_LPAREN T_RPAREN T_LBRACK T_RBRACK

%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

// %type <strval> program header declarations constdefs constant_defs expression variable expressions constant typedefs type_defs type_def dims limits limit sign typename standard_type vardefs variable_defs identifiers subprograms subprogram sub_header formal_parameters parameter_list pass comp_statement statements statement assignment if_statement if_tail while_statement for_statement iter_space subprogram_call io_statement read_list read_item write_list write_item

%start program

%%

program:		header declarations subprograms comp_statement T_DOT
			;

header:		T_PROGRAM T_ID T_SEMI
			| T_PROGRAM T_ID error	{ yyerrok; }
			;

declarations:		constdefs typedefs vardefs
			;
 
constdefs: 		T_CONST constant_defs T_SEMI
			| T_CONST constant_defs error { yyerrok; }
			| %empty { }
			;
					
constant_defs:		constant_defs T_SEMI T_ID { hashtbl_insert(symbolTable, $3, yylineno, scope); } T_EQU expression
			| constant_defs error { yyerrok; } T_ID T_EQU expression
			| T_ID T_EQU expression
			;
 
expression:		expression T_RELOP expression
			| expression T_EQU expression
			| expression T_OROP expression
			| expression T_ADDOP expression
			| expression T_MULDIVANDOP expression
			| T_ADDOP expression
			| T_NOTOP expression
			| variable
			| T_ID T_LPAREN expressions T_RPAREN
			| T_LENGTH T_LPAREN expression T_RPAREN
			| constant
			| T_LPAREN expression T_RPAREN
 			;
 			
variable: 		T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); }
			| variable T_LBRACK expressions T_RBRACK
			;
 
expressions:		expressions T_COMMA expression
			| expression
			;
 
constant:		T_ICONST
			| T_RCONST
			| T_BCONST
			| T_CCONST
			| T_SCONST
			;
			
typedefs:		T_TYPE type_defs T_SEMI
			| T_TYPE type_defs error { yyerrok; }
			| %empty { }
 			;
 			
type_defs:		type_defs T_SEMI T_ID { hashtbl_insert(symbolTable, $3, yylineno, scope); } T_EQU type_def
			| type_defs error { yyerrok; } T_ID T_EQU type_def
			| T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); } T_EQU type_def
 			;
 			
type_def:		T_ARRAY T_LBRACK dims T_RBRACK T_OF typename
			| T_LPAREN identifiers T_RPAREN
			| limit T_DOTDOT limit
 			;
 			
dims:			dims T_COMMA limits
			| limits
			;
 
limits:		limit T_DOTDOT limit
			| T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); }
			;
 
limit:			sign T_ICONST
			| T_CCONST
			| T_BCONST
			| T_ADDOP T_ID
			| T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); }
			;

sign:			T_ADDOP
			| %empty { }
			;
 
typename:		standard_type
			| T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); }
			;
 
standard_type:		T_INTEGER
			| T_REAL
			| T_BOOLEAN
			| T_CHAR
			| T_STRING
			;
 
vardefs:		T_VAR variable_defs T_SEMI
			| T_VAR variable_defs error { yyerrok; }
			| %empty { }
			;
 
variable_defs:		variable_defs T_SEMI identifiers T_COLON typename 
			| variable_defs error { yyerrok; } identifiers T_COLON typename
			| identifiers T_COLON typename
			;
 
identifiers:		identifiers T_COMMA T_ID { hashtbl_insert(symbolTable, $3, yylineno, scope); }
			| T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); }
			;
 
subprograms:		subprograms subprogram T_SEMI
			| subprograms subprogram error { yyerrok; }
			| %empty { }
			;
 
subprogram:		sub_header T_SEMI T_FORWARD
			| sub_header error { yyerrok; } T_FORWARD
			| sub_header T_SEMI declarations subprograms comp_statement
			| sub_header error { yyerrok; } declarations subprograms comp_statement
			;
 
sub_header:		T_FUNCTION T_ID { hashtbl_insert(symbolTable, $2, yylineno, scope); } formal_parameters T_COLON typename
			| T_PROCEDURE T_ID { hashtbl_insert(symbolTable, $2, yylineno, scope); } formal_parameters
			| T_FUNCTION T_ID { hashtbl_insert(symbolTable, $2, yylineno, scope); }
			;
 
formal_parameters:	T_LPAREN parameter_list T_RPAREN
			| %empty { }
			;
 
parameter_list:	parameter_list T_SEMI pass identifiers T_COLON typename
			| parameter_list error { yyerrok; } pass identifiers T_COLON typename
			| pass identifiers T_COLON typename
			;
 
pass:			T_VAR
			| %empty { }
			;
			 
comp_statement:	T_BEGIN { scope++; } statements { hashtbl_get(symbolTable, scope); scope--; } T_END
			;
 
statements:		statements T_SEMI statement
			| statements error { yyerrok; } statement
			| statement
			;
 
statement:		assignment
			| if_statement
			| while_statement
			| for_statement
			| subprogram_call
			| io_statement
			| comp_statement
			| %empty { }
			;

assignment:		variable T_ASSIGN expression 
 			;
 			
if_statement:		T_IF { scope++; } expression T_THEN statement { hashtbl_get(symbolTable, scope); scope--; } if_tail
			;
 
if_tail:		T_ELSE { scope++; } statement { hashtbl_get(symbolTable, scope); scope--; }
			| %empty %prec LOWER_THAN_ELSE { }
			;

while_statement:	T_WHILE { scope++; } expression T_DO statement { hashtbl_get(symbolTable, scope); scope--; }
			;
 
for_statement:		T_FOR T_ID { scope++; hashtbl_insert(symbolTable, $2, yylineno, scope); } T_ASSIGN iter_space T_DO statement { hashtbl_get(symbolTable, scope); scope--; }
			;
 
iter_space:		expression T_TO expression
			| expression T_DOWNTO expression
			;
 
subprogram_call:	T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); }
			| T_ID { hashtbl_insert(symbolTable, $1, yylineno, scope); } T_LPAREN expressions T_RPAREN
			;
 
io_statement:		T_READ T_LPAREN read_list T_RPAREN
			| T_WRITE T_LPAREN write_list T_RPAREN
			;
 
read_list:		read_list T_COMMA read_item  
			| read_item
			;
 
read_item:		variable
			;
 
write_list:		write_list T_COMMA write_item
			| write_item 
			;
 
write_item:		expression
			;

%%

int main(int argc, char* argv[])
{
	if (!(symbolTable = hashtbl_create(13, NULL)))
	{
		printf("\n[ERROR: hashtbl_create() failed. Exiting...]\n");
		exit(EXIT_FAILURE);
	}

	yyin = fopen(argv[1], "r");

	if (yyin == NULL)
	{
		printf("Unable to open file: %s\n", argv[1]);
		return 1;
	}
	
	printf("\n================== [Line %d] ==================\n", yylineno);	
	
	yyparse();
	
	fclose(yyin);
	
	hashtbl_get(symbolTable, scope);
	hashtbl_destroy(symbolTable);

	return 0;
}

void yyerror(const char* err)
{
	errorCounter++;

	printf("\n[ERROR at line %d: %s]\n", yylineno, err);
	
	if (errorCounter == 5)
	{
		printf("\nError count limit reached. Exiting...\n");
		exit(EXIT_FAILURE);
	}
}
