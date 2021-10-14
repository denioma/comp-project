 // José Miguel Rocha Gonçalves - 2019223292
 // Rodrigo Alexandre da Mota Machado - 2019218299
 // -------------------------------------------------

%{
	int col = 1, temp = 0;							// Keep track of line column
	short int semicolon = 0;				// Use for SEMICOLON emission if last token valid
	short int valid = 1;					// Use for token emission after complete match
%}
letter			[A-Za-z_]
number			[0-9]
alnum			{letter}|{number}
identifier		{letter}{alnum}*
integer			[1-9]{number}*
octal			0{integer}
hex			    0[xX][0-9a-fA-F]+
string			([^\n\r\"\\]|\\[fnrt\\\"])*
exponent		[eE][-+]?{number}+
real			({number}+"."({number}+)?{exponent}?) | ({number}+{exponent}) | ("."{number}+{exponent}?)
%X STRLIT OCTAL COMMENT
%%
0               {BEGIN OCTAL;}
{integer}		{fprintf(yyout, "INTLIT(%s)\n", yytext); col += yyleng;}
{hex}           {fprintf(yyout, "INTLIT(%s)\n", yytext); col += yyleng;}
{real}			{fprintf(yyout, "REALLIT(%s)\n", yytext); col += yyleng;}
\"				{++col; BEGIN(STRLIT);}
"//".*			{;}
"/*"            {col +=2; BEGIN COMMENT;}
";"				{fprintf(yyout, "SEMICOLON\n"); col += yyleng;}
","				{fprintf(yyout, "COMMA\n"); col += yyleng;}
"_"				{fprintf(yyout, "BLANKID\n"); col += yyleng;}
"="				{fprintf(yyout, "ASSIGN\n"); col += yyleng;}
"*"				{fprintf(yyout, "STAR\n"); col += yyleng;}
"/"				{fprintf(yyout, "DIV\n"); col += yyleng;}
"-"				{fprintf(yyout, "MINUS\n"); col += yyleng;}
"+"				{fprintf(yyout, "PLUS\n"); col += yyleng;}
"=="			{fprintf(yyout, "EQ\n"); col += yyleng;}
">="			{fprintf(yyout, "GE\n"); col += yyleng;}
">"				{fprintf(yyout, "GT\n"); col += yyleng;}
"{"				{fprintf(yyout, "LBRACE\n"); col += yyleng;}
"<="			{fprintf(yyout, "LE\n"); col += yyleng;}
"("				{fprintf(yyout, "LPAR\n"); col += yyleng;}
"["				{fprintf(yyout, "LSQ\n"); col += yyleng;}
"<"				{fprintf(yyout, "LT\n"); col += yyleng;}
"%"				{fprintf(yyout, "MOD\n"); col += yyleng;}
"!="			{fprintf(yyout, "NE\n"); col += yyleng;}
"!"				{fprintf(yyout, "NOT\n"); col += yyleng;}
"&&"			{fprintf(yyout, "AND\n"); col += yyleng;}
"||"			{fprintf(yyout, "OR\n"); col += yyleng;}
"}"				{fprintf(yyout, "RBRACE\n"); col += yyleng;}
")"				{fprintf(yyout, "RPAR\n"); col += yyleng;}
"]"				{fprintf(yyout, "RSQ\n"); col += yyleng;}
"package"		{fprintf(yyout, "PACKAGE\n"); col += yyleng;}
"return"		{fprintf(yyout, "RETURN\n"); col += yyleng;}
"else"			{fprintf(yyout, "ELSE\n"); col += yyleng;}
"for"			{fprintf(yyout, "FOR\n"); col += yyleng;}
"if"			{fprintf(yyout, "IF\n"); col += yyleng;}
"var"			{fprintf(yyout, "VAR\n"); col += yyleng;}
"int"			{fprintf(yyout, "INT\n"); col += yyleng;}
"float32"		{fprintf(yyout, "FLOAT32\n"); col += yyleng;}
"bool"			{fprintf(yyout, "BOOL\n"); col += yyleng;}
"string"		{fprintf(yyout, "STRING\n"); col += yyleng;}
"fmt.Println"	{fprintf(yyout, "PRINT\n"); col += yyleng;}
"strconv.Atoi"	{fprintf(yyout, "PARSEINT\n"); col += yyleng;}
"func"			{fprintf(yyout, "FUNC\n"); col += yyleng;}
"os.Args"		{fprintf(yyout, "CMDARGS\n"); col += yyleng;}
{identifier}	{fprintf(yyout, "ID(%s)\n", yytext); col += yyleng;}
" "				{++col;}
.				{fprintf(stdout, "Line %d, column %d: illegal character('%c')\n", yylineno, ++col, *yytext);}
\n				{col = 1;}

<STRLIT>{string}\"		{col += yyleng; if (valid) fprintf(yyout, "STRLIT(%s)\n", yytext); valid = 1; BEGIN 0;}
<STRLIT>\\[^fnrt\\\"]	{fprintf(stdout, "Line %d, column %d: invalid escape sequence (%s)\n", yylineno, col, yytext); col += 2; valid = 0;}
<STRLIT>.				{++col;}

<COMMENT>"*/"       {BEGIN 0;}
<COMMENT>.          {++temp;}
<COMMENT>\n         {col = 1;}

<OCTAL>[0-7]*       {fprintf(yyout, "INTLIT(0%s)\n", yytext); col += yyleng; BEGIN 0;}
<OCTAL>[0-9]*       {fprintf(stdout, "Line %d, column %d: invalid octal constant (0%s)\n", yylineno, col, yytext); col += yyleng; BEGIN 0;}
<OCTAL>.|\n         {BEGIN 0;}
%%
int main(int argc, char** argv) {
	yylex();
	return 0;
}

int yywrap() {
	return 1;
}

