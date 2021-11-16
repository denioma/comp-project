#include "tree.h"
extern char* yytext;
extern int yylineno;

extern char lflag, tflag, semicolon;
extern int col, line, temp;

extern int yylex();
extern prog_node* program;