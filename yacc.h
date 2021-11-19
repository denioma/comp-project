#include "tree.h"
extern char* yytext;
extern int yylineno, yyleng;

extern char lflag, tflag, semicolon, build;
extern int col, line, temp, ycol, yline;

extern int yylex();
extern prog_node* program;