%{
	// José Miguel Rocha Gonçalves - 2019223292
	// Rodrigo Alexandre da Mota Machado - 2019218299
	// -------------------------------------------------
	#include "codegen.h"
	#include "tree.h"
	#include "semantic.h"
	#include "symtab.h"
	#include "y.tab.h"
	#include <string.h>

	char lflag = 0, tflag = 0, sflag = 0;
	char semicolon = 0, status = 0, build = 1;
	int col = 1, line = 1, temp = 0, ycol = 1, yline = 1;
	prog_node* program;
	symtab* global;
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
{hex}           {yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); else {yylval.tkn = create_tkn(yytext, yline, ycol); return INTLIT;}}
{octal}         {yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); else {yylval.tkn = create_tkn(yytext, yline, ycol); return INTLIT;}}
0[0-9]*         {status = 1; fprintf(stdout, "Line %d, column %d: invalid octal constant (%s)\n", yylineno, col, yytext); col += yyleng;}
{integer}		{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); else {yylval.tkn = create_tkn(yytext, yline, ycol); return INTLIT;}}
{real}			{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "REALLIT(%s)\n", yytext); else {yylval.tkn = create_tkn(yytext, yline, ycol); return REALLIT;}}
{string}		{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "STRLIT(%s)\n", yytext); else {yylval.tkn = create_tkn(yytext, yline, ycol); return STRLIT;}}
\"				{temp = col; semicolon = 0; BEGIN(BADSTR);}
"//".*			{;}
"/*"            {temp = col + 2; line = yylineno; BEGIN COMMENT;}
";"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}
","				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "COMMA\n"); else return  COMMA;}
"_"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "BLANKID\n"); else return BLANKID;}
"="				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "ASSIGN\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return ASSIGN;}}
"*"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "STAR\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return STAR;}}
"/"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "DIV\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return DIV;}}
"-"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "MINUS\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return MINUS;}}
"+"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PLUS\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return PLUS;}}
"=="			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "EQ\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return EQ;}}
">="			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "GE\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return GE;}}
">"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "GT\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return GT;}}
"{"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LBRACE\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return LBRACE;}}
"<="			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LE\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return LE;}}
"("				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LPAR\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return LPAR;}}
"["				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LSQ\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return LSQ;}}
"<"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "LT\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return LT;}}
"%"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "MOD\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return MOD;}}
"!="			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "NE\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return NE;}}
"!"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "NOT\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return NOT;}}
"&&"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "AND\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return AND;}}
"||"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "OR\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return OR;}}
"}"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RBRACE\n"); else return RBRACE;}
")"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RPAR\n"); else return RPAR;}
"]"				{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RSQ\n"); else return RSQ;}
"package"		{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PACKAGE\n"); else return PACKAGE;}
"return"		{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "RETURN\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return RETURN;}}
"else"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "ELSE\n"); else return ELSE;}
"for"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "FOR\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return FOR;}}
"if"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "IF\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol);  return IF;}}
"var"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "VAR\n"); else return VAR;}
"int"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "INT\n"); else return INT;}
"float32"		{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "FLOAT32\n"); else return FLOAT32;}
"bool"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "BOOL\n"); else return BOOL;}
"string"		{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "STRING\n"); else return STRING;}
"fmt.Println"	{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PRINT\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return PRINT;}}
"strconv.Atoi"	{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "PARSEINT\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return PARSEINT;}}
"func"			{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "FUNC\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return FUNC;}}
"os.Args"		{yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "CMDARGS\n"); else {yylval.tkn = create_tkn(yytext, yline, ycol); return CMDARGS;}}
{reserved}      {yline = yylineno; ycol = col; col += yyleng; semicolon = 0; if (lflag) fprintf(yyout, "RESERVED(%s)\n", yytext); else return RESERVED;}
{identifier}	{yline = yylineno; ycol = col; col += yyleng; semicolon = 1; if (lflag) fprintf(yyout, "ID(%s)\n", yytext); else {yylval.tkn = create_tkn(yytext, yline, ycol); return ID;}}
{whitespace}    {++col;}
{newline}		{yline = yylineno-1; ycol = col; col = 1; if (semicolon) {semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}}
.				{fprintf(stdout, "Line %d, column %d: illegal character (%c)\n", yylineno, col++, *yytext); semicolon = 0;}

<BADSTR>\\				{temp++; status = 1; fprintf(stdout, "Line %d, column %d: invalid escape sequence (\\)\n", yylineno, temp);}
<BADSTR>\\[fnrt\\\"]	{temp += 2;}
<BADSTR>\\.				{temp++; status = 1; fprintf(stdout, "Line %d, column %d: invalid escape sequence (%s)\n", yylineno, temp++, yytext);}
<BADSTR>\"				{col = temp + 2; temp = 1; BEGIN 0;}
<BADSTR>{newline}		{status = 1; fprintf(stdout, "Line %d, column %d: unterminated string literal\n", yylineno-1, col); col = 1; BEGIN 0;}
<BADSTR><<EOF>>			{status = 1; fprintf(stdout, "Line %d, column %d: unterminated string literal\n", yylineno, col); yyterminate();}
<BADSTR>.				{++temp;}

<COMMENT>"*/"       {col = temp + 2; temp = 1; BEGIN 0;}
<COMMENT>.          {++temp;}
<COMMENT>\n         {yline = yylineno; ycol = col; temp = 1; if(semicolon) {semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}}
<COMMENT><<EOF>>	{yline = yylineno; ycol = col; if (semicolon) {semicolon = 0; if (lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;}
					 printf("Line %d, column %d: unterminated comment\n", line, col); yyterminate();}

<INITIAL><<EOF>>	{yline = yylineno; ycol = col; if (semicolon) {semicolon = 0; if(lflag) fprintf(yyout, "SEMICOLON\n"); else return SEMICOLON;} yyterminate();}
%%
int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(*(argv+i), "-l") == 0) lflag = 1;
		if (strcmp(*(argv+i), "-t") == 0) tflag = 1;
		if (strcmp(*(argv+i), "-s") == 0) sflag = 1;
    }

	if (lflag) {
		tflag = 0; sflag = 0;
	} else if (tflag) sflag = 0;

	if (lflag) {
		yylex();
		return status;
	}

	yyparse();
	if (build) {
		if (tflag) {
			print_ast(program);
		} else {
			if (semantic_check(&global, program)) status = 1;
			else {
				if (sflag) {
					show_tables(global, program);
					print_annotations(program);
				} else codegen(program, global);
			}
		}
	} else status = 1;

	// destroy_table(global);
	// destroy(program);
	return status;
}

int yywrap() {
	return 1;
}

