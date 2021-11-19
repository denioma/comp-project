%{
	// José Miguel Rocha Gonçalves - 2019223292
	// Rodrigo Alexandre da Mota Machado - 2019218299
	// -------------------------------------------------
	#include "tree.h"
	#include "y.tab.h"
	#include <string.h>

	char lflag = 0, tflag = 0, semicolon = 0, build = 1;
	int col = 1, line = 1, temp = 0, ycol = 1;
	prog_node* program;
%}
letter			[A-Za-z_]
number			[0-9]
alnum			{letter}|{number}
identifier		{letter}{alnum}*
integer			[1-9]{number}*
octal			0[0-7]*
hex			    0[xX][0-9a-fA-F]+
string			\"([^\n\r\"\\]|\\[fnrt\\\"])*\"
exponent		[eE][-+]?{number}+
real			({number}+"."({number}+)?{exponent}?)|({number}+{exponent})|("."{number}+{exponent}?)
whitespace      [" "\t]
newline			\r\n|\n
reserved        break|case|chan|const|continue|default|defer|fallthrough|go|goto|import|interface|map|range|select|struct|switch|type|"++"|"--"
%option yylineno
%x BADSTR COMMENT
%%
{hex}           {ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); else {yylval.token = strdup(yytext); return INTLIT;}}
{octal}         {ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); else {yylval.token = strdup(yytext); return INTLIT;}}
0[0-9]*         {printf("Line %d, column %d: invalid octal constant (%s)\n", yylineno, col, yytext); col += yyleng;}
{integer}		{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); else {yylval.token = strdup(yytext); return INTLIT;}}
{real}			{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "REALLIT(%s)\n", yytext); else {yylval.token = strdup(yytext); return REALLIT;}}
{string}		{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "STRLIT(%s)\n", yytext); else {yylval.token = strdup(yytext); return STRLIT;}}
\"				{temp = col; semicolon = 0; BEGIN(BADSTR);}
"//".*			{;}
"/*"            {temp = col + 2; line = yylineno; BEGIN COMMENT;}
";"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}
","				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "COMMA\n"); else return  COMMA;}
"_"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "BLANKID\n"); else return BLANKID;}
"="				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "ASSIGN\n"); else return ASSIGN;}
"*"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "STAR\n"); else return STAR;}
"/"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "DIV\n"); else return DIV;}
"-"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "MINUS\n"); else return MINUS;}
"+"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PLUS\n"); else return PLUS;}
"=="			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "EQ\n"); else return EQ;}
">="			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "GE\n"); else return GE;}
">"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "GT\n"); else return GT;}
"{"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LBRACE\n"); else return LBRACE;}
"<="			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LE\n"); else return LE;}
"("				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LPAR\n"); else return LPAR;}
"["				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LSQ\n"); else return LSQ;}
"<"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LT\n"); else return LT;}
"%"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "MOD\n"); else return MOD;}
"!="			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "NE\n"); else return NE;}
"!"				{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "NOT\n"); else return NOT;}
"&&"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "AND\n"); else return AND;}
"||"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "OR\n"); else return OR;}
"}"				{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RBRACE\n"); else return RBRACE;}
")"				{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RPAR\n"); else return RPAR;}
"]"				{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RSQ\n"); else return RSQ;}
"package"		{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PACKAGE\n"); else return PACKAGE;}
"return"		{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RETURN\n"); else return RETURN;}
"else"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "ELSE\n"); else return ELSE;}
"for"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "FOR\n"); else return FOR;}
"if"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "IF\n"); else return IF;}
"var"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "VAR\n"); else return VAR;}
"int"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "INT\n"); else return INT;}
"float32"		{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "FLOAT32\n"); else return FLOAT32;}
"bool"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "BOOL\n"); else return BOOL;}
"string"		{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "STRING\n"); else return STRING;}
"fmt.Println"	{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PRINT\n"); else return PRINT;}
"strconv.Atoi"	{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PARSEINT\n"); else return PARSEINT;}
"func"			{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "FUNC\n"); else return FUNC;}
"os.Args"		{ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "CMDARGS\n"); else return CMDARGS;}
{reserved}      {ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "RESERVED(%s)\n", yytext); else return RESERVED;}
{identifier}	{ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "ID(%s)\n", yytext); else {yylval.token = strdup(yytext); return ID;}}
{whitespace}    {++col;}
{newline}		{ycol = col; col = 1; if (semicolon) {semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}}
.				{fprintf(stdout, "Line %d, column %d: illegal character (%c)\n", yylineno, col++, *yytext); semicolon = 0;}

<BADSTR>\\				{temp++; fprintf(stdout, "Line %d, column %d: invalid escape sequence (\\)\n", yylineno, temp);}
<BADSTR>\\[fnrt\\\"]	{temp += 2;}
<BADSTR>\\.				{temp++; fprintf(stdout, "Line %d, column %d: invalid escape sequence (%s)\n", yylineno, temp++, yytext);}
<BADSTR>\"				{col = temp + 2; temp = 1; BEGIN 0;}
<BADSTR>{newline}		{fprintf(stdout, "Line %d, column %d: unterminated string literal\n", yylineno-1, col); col = 1; BEGIN 0;}
<BADSTR><<EOF>>			{fprintf(stdout, "Line %d, column %d: unterminated string literal\n", yylineno, col); yyterminate();}
<BADSTR>.				{++temp;}

<COMMENT>"*/"       {col = temp + 2; temp = 1; BEGIN 0;}
<COMMENT>.          {++temp;}
<COMMENT>\n         {temp = 1;}
<COMMENT><<EOF>>	{ycol = col; if (semicolon) {semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}
					 printf("Line %d, column %d: unterminated comment\n", line, col); yyterminate();}

<INITIAL><<EOF>>	{ycol = col; if (semicolon) {semicolon = 0; if(lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;} yyterminate();}
%%
int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(*(argv+i), "-l") == 0) lflag = 1;
		if (strcmp(*(argv+i), "-t") == 0) tflag = 1;
    }
	if (lflag) yylex();
	else {
		yyparse();
		if (tflag && build) print_ast(program);
	}
	return 0;
}

int yywrap() {
	return 1;
}

