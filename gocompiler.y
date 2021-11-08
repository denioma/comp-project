%{
#include <stdio.h>
int yylex();
void yyerror(const char*);
extern int col;
%}
%token INTLIT STRLIT REALLIT
%token SEMICOLON COMMA BLANKID ASSIGN ID VAR
%token PLUS MINUS STAR DIV MOD
%token EQ GE GT LE LT NE 
%token NOT AND OR
%token LPAR RPAR LBRACE RBRACE LSQ RSQ
%token PACKAGE RETURN PRINT PARSEINT FUNC CMDARGS RESERVED
%token IF ELSE FOR 
%token INT FLOAT32 BOOL STRING

%left OR
%left AND
%left EQ GE GT LE LT NE
%left MOD
%left PLUS MINUS
%left STAR DIV
%right ASSIGN
%nonassoc LPAR RPAR
%nonassoc NOT
%%

Program: PACKAGE ID SEMICOLON Declarations ;

Declarations:
        Declarations VarDeclaration SEMICOLON
    |   Declarations FuncDeclaration SEMICOLON
    |    /* empty */
    ;

VarDeclaration: 
        VAR VarSpec 
    |   VAR LPAR VarSpec SEMICOLON RPAR

FuncDeclaration: FUNC ID LPAR FuncParams RPAR FuncType FuncBody

FuncParams: Parameters | /* empty */

FuncType: Type | /* empty */

VarSpec: ID IdReps Type ;

IdReps: IdReps COMMA ID | /* empty */ ;
        
Type: INT | FLOAT32 | BOOL | STRING ;

Parameters: ID Type ParamOpts ;

ParamOpts: ParamOpts COMMA ID Type | /* empty */ ;

FuncBody: LBRACE VarsAndStatements RBRACE ;

VarsAndStatements: 
        VarsAndStatements VASOpts SEMICOLON 
    |   /* empty */ ;

VASOpts: VarDeclaration | Statement | /* empty */ ;

Statement:
        ID ASSIGN Expr
    |   LBRACE StmtBlock RBRACE
    |   IF Expr LBRACE StmtBlock RBRACE ElseStmt
    |   FOR ExprOpt LBRACE StmtBlock RBRACE
    |   RETURN ExprOpt
    |   FuncInvocation
    |   ParseArgs
    |   PRINT LPAR STRLIT RPAR
    |   PRINT LPAR Expr RPAR ;

StmtBlock: StmtBlock Statement SEMICOLON | /* empty */ ;

ElseStmt: ELSE LBRACE StmtBlock RBRACE | /* empty */ ;

ExprOpt: Expr | /* empty */ ;

ParseArgs: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR

FuncInvocation: ID LPAR FuncInvocationOpts RPAR

FuncInvocationOpts: Expr ExprReps | /* empty */ ;
    
ExprReps: ExprReps COMMA Expr | /* empty */ ;

Expr:
       Expr ExprOperators Expr
    |   NOT Expr
    |   INTLIT
    |   REALLIT
    |   ID
    |   FuncInvocation
    |   LPAR Expr RPAR ;

ExprOperators: OR | AND | LT | GT | EQ | NE | LE | GE |
            PLUS | MINUS | STAR | DIV | MOD ;

%%
int main() {
    yylex();
    return 0;
}

int yyerror(char* s) {
    printf("Line %d, column %d: %s: %s\n", yylineno, col, s, yytext);
}