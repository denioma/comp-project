 // José Miguel Rocha Gonçalves - 2019223292
 // Rodrigo Alexandre da Mota Machado - 2019218299
 // -------------------------------------------------

%{
    #include <string.h>
    int lflag = 0; 
	int col = 1, temp = 0, line = 1;		// Keep track of line column
	short int semicolon = 0;				// Use for SEMICOLON emission if last token valid
	short int valid = 1;					// Use for token emission after complete match
%}
letter			[A-Za-z_]
number			[0-9]
alnum			{letter}|{number}
identifier		{letter}{alnum}*
integer			[1-9]{number}*
hex			    0[xX][0-9a-fA-F]+
string			([^\n\r\"\\]|\\[fnrt\\\"])*
whitespace      [" "\t]
reserved        break|case|chan|const|continue|default|defer|fallthrough|go|goto|import|interface|map|range|select|struct|switch|type|"++"|"--"
%X STRLIT OCTAL COMMENT
%%
{hex}           {if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); col += yyleng;}
0[0-7]*         {if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); col += yyleng;}
0[0-9]*         {printf("Line %d, column %d: invalid octal constant (%s)\n", yylineno, col, yytext); col += yyleng;}
{integer}		{if (lflag) fprintf(yyout, "INTLIT(%s)\n", yytext); col += yyleng;}
\"				{++col; BEGIN(STRLIT);}
"//".*			{;}
"/*"            {temp = col; line = yylineno; BEGIN COMMENT;}
";"				{if (lflag) fprintf(yyout, "SEMICOLON\n"); col += yyleng;}
","				{if (lflag) fprintf(yyout, "COMMA\n"); col += yyleng;}
"_"				{if (lflag) fprintf(yyout, "BLANKID\n"); col += yyleng;}
"="				{if (lflag) fprintf(yyout, "ASSIGN\n"); col += yyleng;}
"*"				{if (lflag) fprintf(yyout, "STAR\n"); col += yyleng;}
"/"				{if (lflag) fprintf(yyout, "DIV\n"); col += yyleng;}
"-"				{if (lflag) fprintf(yyout, "MINUS\n"); col += yyleng;}
"+"				{if (lflag) fprintf(yyout, "PLUS\n"); col += yyleng;}
"=="			{if (lflag) fprintf(yyout, "EQ\n"); col += yyleng;}
">="			{if (lflag) fprintf(yyout, "GE\n"); col += yyleng;}
">"				{if (lflag) fprintf(yyout, "GT\n"); col += yyleng;}
"{"				{if (lflag) fprintf(yyout, "LBRACE\n"); col += yyleng;}
"<="			{if (lflag) fprintf(yyout, "LE\n"); col += yyleng;}
"("				{if (lflag) fprintf(yyout, "LPAR\n"); col += yyleng;}
"["				{if (lflag) fprintf(yyout, "LSQ\n"); col += yyleng;}
"<"				{if (lflag) fprintf(yyout, "LT\n"); col += yyleng;}
"%"				{if (lflag) fprintf(yyout, "MOD\n"); col += yyleng;}
"!="			{if (lflag) fprintf(yyout, "NE\n"); col += yyleng;}
"!"				{if (lflag) fprintf(yyout, "NOT\n"); col += yyleng;}
"&&"			{if (lflag) fprintf(yyout, "AND\n"); col += yyleng;}
"||"			{if (lflag) fprintf(yyout, "OR\n"); col += yyleng;}
"}"				{if (lflag) fprintf(yyout, "RBRACE\n"); col += yyleng;}
")"				{if (lflag) fprintf(yyout, "RPAR\n"); col += yyleng;}
"]"				{if (lflag) fprintf(yyout, "RSQ\n"); col += yyleng;}
"package"		{if (lflag) fprintf(yyout, "PACKAGE\n"); col += yyleng;}
"return"		{if (lflag) fprintf(yyout, "RETURN\n"); col += yyleng;}
"else"			{if (lflag) fprintf(yyout, "ELSE\n"); col += yyleng;}
"for"			{if (lflag) fprintf(yyout, "FOR\n"); col += yyleng;}
"if"			{if (lflag) fprintf(yyout, "IF\n"); col += yyleng;}
"var"			{if (lflag) fprintf(yyout, "VAR\n"); col += yyleng;}
"int"			{if (lflag) fprintf(yyout, "INT\n"); col += yyleng;}
"float32"		{if (lflag) fprintf(yyout, "FLOAT32\n"); col += yyleng;}
"bool"			{if (lflag) fprintf(yyout, "BOOL\n"); col += yyleng;}
"string"		{if (lflag) fprintf(yyout, "STRING\n"); col += yyleng;}
"fmt.Println"	{if (lflag) fprintf(yyout, "PRINT\n"); col += yyleng;}
"strconv.Atoi"	{if (lflag) fprintf(yyout, "PARSEINT\n"); col += yyleng;}
"func"			{if (lflag) fprintf(yyout, "FUNC\n"); col += yyleng;}
"os.Args"		{if (lflag) fprintf(yyout, "CMDARGS\n"); col += yyleng;}
{reserved}      {if (lflag) fprintf(yyout, "RESERVED(%s)\n", yytext);}
{identifier}	{if (lflag) fprintf(yyout, "ID(%s)\n", yytext); col += yyleng;}
{whitespace}    {++col;}
\n|\r\n			{col = 1; yylineno++;}
.				{fprintf(stdout, "Line %d, column %d: illegal character('%c')\n", yylineno, ++col, *yytext);}

<STRLIT>{string}\"		{col += yyleng; if (valid && lflag) fprintf(yyout, "STRLIT(\"%s)\n", yytext); valid = 1; BEGIN 0;}
<STRLIT>\\[^fnrt\\\"]	{fprintf(stdout, "Line %d, column %d: invalid escape sequence (%s)\n", yylineno, col, yytext); col += 2; valid = 0;}
<STRLIT>.				{++col;}

<COMMENT>"*/"       {BEGIN 0;}
<COMMENT>.          {++temp;}
<COMMENT>\n         {temp = 1;}
<COMMENT><<EOF>>	{printf("Line %d, column %d: unterminated comment\n", line, col); BEGIN 0;}
%%
int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(*(argv+i), "-l") == 0) lflag = 1;
    }
	yylex();
	return 0;
}

int yywrap() {
	return 1;
}

