%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <math.h>
	#include <string.h>
	#include "parser.tab.h"

	char str_buf[256];
	char str_esc[256];
	
	int strtoint(const char* str);
	char strtochar(const char* str);
	void stresc(char* original_str, char* final_str);
	float strtoreal(const char* str);
	
	extern void yyerror(const char* err);
%}

%option 	case-insensitive
%option 	yylineno
%option 	noyywrap noinput nounput

%x 		STRING
%x 		COMMENT

LETTER		[a-zA-Z]
DIGIT		[0-9]
NZDIGIT	[1-9]

ID		_?{LETTER}([a-zA-Z0-9]*|_+[a-zA-Z0-9]+)*
ICONST		0|([1-9][0-9]*)|(0[xX][1-9A-F][0-9A-F]*)
RCONST		((0?|([1-9][0-9]*))\.(([0-9]*[1-9])|0)([eE][+-]?(([1-9][0-9]*)|0))?)|((([1-9][0-9]*)|0)[eE][+-]?(([1-9][0-9]*)|0))|(0[xX](([1-9A-F][0-9A-F]*)|0?)\.(([0-9A-F]*[1-9A-F])|0))
BCONST		true|false
CCONST		('[ -~]')|('\\[bfnrtv]')
WHITESPACE	[ \t]

%%

	/* Keywords */
"program"			{ printf("\n[PROGRAM]\n");	yylval.strval = strdup(yytext); return T_PROGRAM; }
"const"			{ printf("\n[CONST]\n");	yylval.strval = strdup(yytext); return T_CONST; }
"type"				{ printf("\n[TYPE]\n");	yylval.strval = strdup(yytext); return T_TYPE; }
"array"			{ printf("\n[ARRAY]\n");	yylval.strval = strdup(yytext); return T_ARRAY; }
"of"				{ printf("\n[OF]\n");		yylval.strval = strdup(yytext); return T_OF; }
"var"				{ printf("\n[VAR]\n");		yylval.strval = strdup(yytext); return T_VAR; }
"forward"			{ printf("\n[FORWARD]\n");	yylval.strval = strdup(yytext); return T_FORWARD; }
"function"			{ printf("\n[FUNCTION]\n");	yylval.strval = strdup(yytext); return T_FUNCTION; }
"procedure"			{ printf("\n[PROCEDURE]\n");	yylval.strval = strdup(yytext); return T_PROCEDURE; }
"integer"			{ printf("\n[INTEGER]\n");	yylval.strval = strdup(yytext); return T_INTEGER; }
"real"				{ printf("\n[REAL]\n");	yylval.strval = strdup(yytext); return T_REAL; }
"boolean"			{ printf("\n[BOOLEAN]\n");	yylval.strval = strdup(yytext); return T_BOOLEAN; }
"char"				{ printf("\n[CHAR]\n");	yylval.strval = strdup(yytext); return T_CHAR; }
"string"			{ printf("\n[STRING]\n");	yylval.strval = strdup(yytext); return T_STRING; }
"begin"			{ printf("\n[BEGIN]\n");	yylval.strval = strdup(yytext); return T_BEGIN; }
"end"				{ printf("\n[END]\n");		yylval.strval = strdup(yytext); return T_END; }
"if"				{ printf("\n[IF]\n");		yylval.strval = strdup(yytext); return T_IF; }
"then"				{ printf("\n[THEN]\n");	yylval.strval = strdup(yytext); return T_THEN; }
"else"				{ printf("\n[ELSE]\n");	yylval.strval = strdup(yytext); return T_ELSE; }
"case"				{ printf("\n[CASE]\n");	yylval.strval = strdup(yytext); return T_CASE; }
"otherwise"			{ printf("\n[OTHERWISE]\n");	yylval.strval = strdup(yytext); return T_OTHERWISE; }
"while"			{ printf("\n[WHILE]\n");	yylval.strval = strdup(yytext); return T_WHILE; }
"do"				{ printf("\n[DO]\n");		yylval.strval = strdup(yytext); return T_DO; }
"for"				{ printf("\n[FOR]\n");		yylval.strval = strdup(yytext); return T_FOR; }
"downto"			{ printf("\n[DOWNTO]\n");	yylval.strval = strdup(yytext); return T_DOWNTO; }
"to"				{ printf("\n[TO]\n");		yylval.strval = strdup(yytext); return T_TO; }
"read"				{ printf("\n[READ]\n");	yylval.strval = strdup(yytext); return T_READ; }
"write"			{ printf("\n[WRITE]\n");	yylval.strval = strdup(yytext); return T_WRITE; }
"length"			{ printf("\n[LENGTH]\n");	yylval.strval = strdup(yytext); return T_LENGTH; }

	/* Operators */
">"|">="|"<"|"<="|"<>"		{ printf("\n[RELOP] -> %s\n", yytext);	yylval.strval = strdup(yytext); return T_RELOP; }
"+"|"-"			{ printf("\n[ADDOP] -> %s\n", yytext);	yylval.strval = strdup(yytext); return T_ADDOP; }
"OR"				{ printf("\n[OROP]\n");			yylval.strval = strdup(yytext); return T_OROP; }
"*"|"/"|"DIV"|"MOD"|"AND"	{ printf("\n[MULDIVANDOP] -> %s\n", yytext);	yylval.strval = strdup(yytext); return T_MULDIVANDOP; }
"NOT"				{ printf("\n[NOTOP]\n");			yylval.strval = strdup(yytext); return T_NOTOP; }

	/* Independent LUs */
"("				{ printf("\n[LPAREN]\n");	yylval.strval = strdup(yytext); return T_LPAREN; }
")"				{ printf("\n[RPAREN]\n");	yylval.strval = strdup(yytext); return T_RPAREN; }
";"				{ printf("\n[SEMI]\n");	yylval.strval = strdup(yytext); return T_SEMI; }
"."				{ printf("\n[DOT]\n");		yylval.strval = strdup(yytext); return T_DOT; }
","				{ printf("\n[COMMA]\n");	yylval.strval = strdup(yytext); return T_COMMA; }
"="				{ printf("\n[EQU]\n");		yylval.strval = strdup(yytext); return T_EQU; }
":"				{ printf("\n[COLON]\n");	yylval.strval = strdup(yytext); return T_COLON; }
"["				{ printf("\n[LBRACK]\n");	yylval.strval = strdup(yytext); return T_LBRACK; }
"]"				{ printf("\n[RBRACK]\n");	yylval.strval = strdup(yytext); return T_RBRACK; }
":="				{ printf("\n[ASSIGN]\n");	yylval.strval = strdup(yytext); return T_ASSIGN; }
".."				{ printf("\n[DOTDOT]\n");	yylval.strval = strdup(yytext); return T_DOTDOT; }

	/* Boolean constant */
{BCONST}			{ printf("\n[BCONST] -> %s\n", yytext); yylval.strval = strdup(yytext); return T_BCONST; }

	/* Identifier */
{ID}				{ printf("\n[ID] -> %s\n", yytext);	yylval.strval = strdup(yytext); return T_ID; }

	/* Constants */
{ICONST}			{ printf("\n[ICONST] -> %s -> %d\n", yytext, strtoint(yytext));	yylval.intval = strtoint(yytext);	return T_ICONST; }
{RCONST}			{ printf("\n[RCONST] -> %s\n", yytext);				yylval.realval = atof(yytext);	return T_RCONST; }
{CCONST}			{ printf("\n[CCONST] -> %s -> %c\n", yytext, strtochar(yytext));	yylval.charval = yytext[1];		return T_CCONST; }

	/* String state */
\"				{ strcpy(str_buf, "");	memset(str_esc, 0, sizeof(str_esc)); BEGIN(STRING); }
<STRING>\"			{ stresc(str_buf, str_esc); printf("\n[SCONST] -> %s -> %s\n", str_buf, str_esc);  BEGIN(INITIAL); yylval.strval = strdup(str_buf); return T_SCONST; }
<STRING>\n			{ printf("\nNewline without \\\n"); }
<STRING>\\\n			{ }
<STRING>\\[ ]			{ printf("\n\\ in string but not at the end.\n"); }
<STRING>\\[^ ]			{ strcat(str_buf, yytext); }
<STRING>[^"\\]			{ strcat(str_buf, yytext); }
<STRING><<EOF>>		{ printf("\nEOF in the string.\n"); }
<STRING>.			{ printf("\nUnknown character in string.\n"); }

	/* Comment state */
\{				{ strcpy(str_buf, "");	BEGIN(COMMENT); }
<COMMENT>\}			{ printf("\n[COMMENT] -> %s\n", str_buf); BEGIN(INITIAL); }
<COMMENT>\\n[ ]*		{ }
<COMMENT>\\[ ]			{ strcat(str_buf, yytext); }
<COMMENT>[^\\]			{ strcat(str_buf, yytext); }
<COMMENT><<EOF>>		{ printf("\nEOF in the string.\n"); }

	/* EOF and Misc */
{WHITESPACE}			{ }
\n				{ printf("\n================== [Line %d] ==================\n", yylineno); }
<<EOF>>			{ printf("\n[EOF]\n");	return T_EOF; }
.				{ printf("\nUnrecognized character\n"); }

%%

int strtoint(const char* str)
{
	if (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0)
	{	
		return (int)strtoll(&str[2], NULL, 16);
	}
	else
	{
		return (int)strtoll(str, NULL, 10);
	}
}

float strtoreal(const char* str)
{
	int i = 0;
	int ExpPos = 0;
	int SignPos = 0;
	
	if (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0)
	{
		return (float)strtoll(&str[2], NULL, 16);
	}
	
	while (str[i] != '\0')
	{
		if (str[i] == 'e' || str[i] == 'E')
		{
			ExpPos = i;
			i++;
			
			if (str[i] == '+' || str[i] == '-')
			{
				SignPos = i;
			}
			
			break;
		}
		
		i++;
	}
	
	if (ExpPos == 0)
	{
		return strtof(str, NULL);
	}
}

char strtochar(const char* str)
{
	if (str[1] == '\\')
	{
		switch (str[2])
		{
			case 'b': return '\b';
			case 'f': return '\f';
			case 'n': return '\n';
			case 'r': return '\r';
			case 't': return '\t';
			case 'v': return '\v';
			default:
				yyerror("Unknown escaped character");
				return '\0';
		}
	}
	else
	{
		return str[1];
	}
}

void stresc(char* original_str, char* final_str)
{
	int i, j;
	
	for (i = 0, j = 0; i < strlen(original_str); i++, j++)
	{
		if (original_str[i] == '\\')
		{
			switch (original_str[i + 1])
			{
				case 'b': final_str[j] = '\b'; break;
				case 'f': final_str[j] = '\f'; break;
				case 'n': final_str[j] = '\n'; break;
				case 'r': final_str[j] = '\r'; break;
				case 't': final_str[j] = '\t'; break;
				case 'v': final_str[j] = '\v'; break;
				case '\\': final_str[j] = '\\'; break;
				case '\'': final_str[j] = '\''; break;
				case '"': final_str[j] = '"'; break;
				default:
					yyerror("Unknown escaped character");
					return ;
			}
			
			i++;
		}
		else
		{
			final_str[j] = original_str[i];
		}
	}
}
