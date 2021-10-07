 // José Miguel Rocha Gonçalves - 2019223292
 // Rodrigo Alexandre da Mota Machado - 2019218299
 // -------------------------------------------------

%{
	int checkOctal(char*, int*, int*);
	int checkHex(char*, int*, int*);
	int col;
%}
letter			[A-Za-z]
number			[0-9]
alnum			{letter}|{number}
identifier		{letter}{alnum}*
integer			{number}+
octal			^0{integer}
hex				^0x{integer}
%%
{octal}			{if (!checkOctal(yytext, &yyleng, &yylineno)) fprintf(yyout, "INTLIT(%s)\n", yytext);}
 /* {hex}			{if (!checkHex(yytext, &leng, &yylineno)) fprintf(yyout, "INTLIT(%s)\n", yytext);} */
{integer}		{fprintf(yyout, "INTLIT(%s)\n", yytext);}
";"				{fprintf(yyout, "SEMICOLON\n");}
","				{fprintf(yyout, "COMMA\n");}
"_"				{fprintf(yyout, "BLANKID\n");}
"="				{fprintf(yyout, "ASSIGN\n");}
"*"				{fprintf(yyout, "STAR\n");}
"/"				{fprintf(yyout, "DIV\n");}
"-"				{fprintf(yyout, "MINUS\n");}
"+"				{fprintf(yyout, "PLUS\n");}
"=="			{fprintf(yyout, "EQ\n");}
">="			{fprintf(yyout, "GE\n");}
">"				{fprintf(yyout, "GT\n");}
"{"				{fprintf(yyout, "LBRACE\n");}
"<="			{fprintf(yyout, "LE\n");}
"("				{fprintf(yyout, "LPAR\n");}
"["				{fprintf(yyout, "LSQ\n");}
"<"				{fprintf(yyout, "LT\n");}
"%"				{fprintf(yyout, "MOD\n");}
"!="			{fprintf(yyout, "NE\n");}
"!"				{fprintf(yyout, "NOT\n");}
"&&"			{fprintf(yyout, "AND\n");}
"||"			{fprintf(yyout, "OR\n");}
"}"				{fprintf(yyout, "RBRACE\n");}
")"				{fprintf(yyout, "RPAR\n");}
"]"				{fprintf(yyout, "RSQ\n");}
"package"		{fprintf(yyout, "PACKAGE\n");}
"return"		{fprintf(yyout, "RETURN\n");}
"else"			{fprintf(yyout, "ELSE\n");}
"for"			{fprintf(yyout, "FOR\n");}
"if"			{fprintf(yyout, "IF\n");}
"var"			{fprintf(yyout, "VAR\n");}
"int"			{fprintf(yyout, "INT\n");}
"float32"		{fprintf(yyout, "FLOAT32\n");}
"bool"			{fprintf(yyout, "BOOL\n");}
"string"		{fprintf(yyout, "STRING\n");}
"fmt.Println"	{fprintf(yyout, "PRINT\n");}
"strconv.Atoi"	{fprintf(yyout, "PARSEINT\n");}
"func"			{fprintf(yyout, "FUNC\n");}
"os.Args"		{fprintf(yyout, "CMDARGS\n");}
{identifier}	{fprintf(yyout, "ID(%s)\n", yytext);}
.				{;}
\n				{;}
%%
int main(int argc, char** argv) {
	yylex();
	return 0;
}

int yywrap() {
	return 1;
}

int checkOctal(char* input, int* len, int* line) {
	short int ret = 0;
	for (int i = 1; i < *len; i++) {
		if (*(input+i) - 48 > 7 || *(input+i) - 48 < 0) {
			// Print error msg to stdout
			fprintf(stdout, "Line %d, column %d: invalid octal constant (%c)\n", yylineno, i+1, *(input+i));			
			ret = 1;
		}
	}
	return ret;
}

int checkHex(char* input, int* len, int* line) {
	short int ret = 0;
	for (int i = 1; i < *len; i++) {
		if (*(input+i) - 48 > 7 || *(input+i) - 48 < 0) {
			// Print error msg to stdout
			fprintf(stdout, "Line %d, column %d: invalid octal constant (%c)\n", yylineno, i+1, *(input+i));			
			ret = 1;
		}
	}
	return ret;
}