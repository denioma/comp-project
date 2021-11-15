%{
#include "shared.h"
#include "y.tab.h"
#include <stdio.h>
int yylex();
void yyerror(const char*);
char build = 1;
%}
%union{
    char* token;
}

%token INTLIT STRLIT REALLIT ID
%token SEMICOLON COMMA BLANKID ASSIGN VAR
%token PLUS MINUS STAR DIV MOD
%token EQ GE GT LE LT NE 
%token NOT AND OR
%token LPAR RPAR LBRACE RBRACE LSQ RSQ
%token PACKAGE RETURN PRINT PARSEINT FUNC CMDARGS RESERVED
%token IF ELSE FOR 
%token INT FLOAT32 BOOL STRING

%type Program
%type Declarations VarDeclaration VarSpec
%type <token> Type

%right ASSIGN
%left OR
%left AND
%left EQ GE GT LE LT NE
%left MOD
%left PLUS MINUS
%left STAR DIV
%nonassoc LPAR RPAR
%nonassoc NOT
%%

Program: PACKAGE ID SEMICOLON Declarations      {printf("Parsing complete!\n"); program = new_prog($4);} ;

Declarations:
        Declarations VarDeclaration SEMICOLON   {printf("VarDec\n");}
    |   Declarations FuncDeclaration SEMICOLON  {printf("FuncDec\n");}
    |   /* empty */                             {;} ;

VarDeclaration: 
        VAR VarSpec                             {;}
    |   VAR LPAR VarSpec SEMICOLON RPAR         {;} ;

FuncDeclaration: FUNC ID LPAR FuncParams RPAR FuncType FuncBody {;} ;

FuncParams: Parameters {;} | /* empty */ {;} ;

FuncType: Type {;} | /* empty */ {;} ;

VarSpec: ID IdReps Type {;} ;

IdReps: IdReps COMMA ID {;} | /* empty */ {;} ;
        
Type: INT {;} | FLOAT32 {;} | BOOL {;} | STRING {;} ;

Parameters: ID Type ParamOpts {;} ;

ParamOpts: ParamOpts COMMA ID Type {;} | /* empty */ ;

FuncBody: LBRACE VarsAndStatements RBRACE {;} ;

VarsAndStatements: 
        VarsAndStatements VASOpts SEMICOLON {;}
    |   /* empty */ {;} ;

VASOpts: VarDeclaration {;} | Statement {;}| /* empty */ {;} ;

Statement:
        error {;}
    |   ID ASSIGN Expr {;}
    |   LBRACE StmtBlock RBRACE {;}
    |   IF Expr LBRACE StmtBlock RBRACE ElseStmt {;}
    |   FOR ExprOpt LBRACE StmtBlock RBRACE {;}
    |   RETURN ExprOpt {;}
    |   FuncInvocation {;}
    |   ParseArgs {;}
    |   PRINT LPAR STRLIT RPAR {;}
    |   PRINT LPAR Expr RPAR {;} ;

StmtBlock: StmtBlock Statement SEMICOLON {;} | /* empty */ {;} ;

ElseStmt: ELSE LBRACE StmtBlock RBRACE {;} | /* empty */ {;} ;

ExprOpt: Expr {;} | /* empty */ {;} ;

ParseArgs: 
        ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR {;}
    |   ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ error RSQ RPAR {;} ;


FuncInvocation: 
        ID LPAR FuncInvocationOpts RPAR {;}
    |   ID LPAR error RPAR {;} ;

FuncInvocationOpts: Expr ExprReps {;} | /* empty */ {;} ;
    
ExprReps: ExprReps COMMA Expr {;} | /* empty */ {;} ;

Expr:
        Expr PLUS Expr {;}
    |   Expr MINUS Expr {;}
    |   Expr STAR Expr {;}
    |   Expr DIV Expr {;}
    |   Expr MOD Expr {;}
    |   Expr OR Expr {;}
    |   Expr AND Expr {;}
    |   Expr LT Expr {;}
    |   Expr GT Expr {;}
    |   Expr EQ Expr {;}
    |   Expr GE Expr {;}
    |   Expr LE Expr {;}
    |   Expr NE Expr {;}
    |   NOT Expr {;}
    |   INTLIT {;}
    |   REALLIT {;}
    |   ID {;}
    |   FuncInvocation {;}
    |   LPAR Expr RPAR {;}
    |   LPAR error RPAR {;} ;
%%
void yyerror(const char* s) {
    printf("Line %d, column %d: %s: %s\n", yylineno, col, s, yytext);
    build = 0;
}