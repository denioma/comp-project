%{
    // José Miguel Rocha Gonçalves - 2019223292
	// Rodrigo Alexandre da Mota Machado - 2019218299
	// -------------------------------------------------
    #include "yacc.h"
    #include "y.tab.h"
    #include <stdio.h>
    void yyerror(const char*);
    prog_node* program;
%}
%union{
    char* token;
    dec_node* decl;
    var_dec* var;
    func_dec* func;
    v_type var_type;
    prog_node* prog;
    param_dec* params;
    func_body* f_body;
}

%token <token> INTLIT STRLIT REALLIT ID
%token SEMICOLON COMMA BLANKID ASSIGN VAR
%token PLUS MINUS STAR DIV MOD
%token EQ GE GT LE LT NE 
%token NOT AND OR
%token LPAR RPAR LBRACE RBRACE LSQ RSQ
%token PACKAGE RETURN PRINT PARSEINT FUNC CMDARGS
%token IF ELSE FOR 
%token INT FLOAT32 BOOL STRING
%token RESERVED

%type <prog> Program
%type <decl> Declarations 
%type <var> VarDeclaration VarSpec
%type <var_type> Type FuncType
%type <func> FuncDeclaration
%type <params> FuncParams Parameters ParamOpts
%type <f_body> FuncBody

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

Program: PACKAGE ID SEMICOLON Declarations      {$$=program=new_prog($4);} ;

Declarations:
        Declarations VarDeclaration SEMICOLON   {$$=insert_var_dec($1, $2);}
    |   Declarations FuncDeclaration SEMICOLON  {$$=insert_func_dec($1, $2);}
    |   /* empty */                             {$$=NULL;} ;

VarDeclaration: 
        VAR VarSpec                             {$$=$2;}
    |   VAR LPAR VarSpec SEMICOLON RPAR         {$$=$3;} ;

FuncDeclaration: FUNC ID LPAR FuncParams RPAR FuncType FuncBody {$$=create_func($2, $6, $4);} ;

FuncParams: Parameters {$$=$1;} | /* empty */ {$$=NULL;} ;

FuncType: Type {$$=$1;} | /* empty */ {$$=v_void;} ;

VarSpec: ID IdReps Type {$$=create_var($1, $3);} ;

IdReps: IdReps COMMA ID {;} | /* empty */ {;} ;
        
Type: INT {$$=v_int;} | FLOAT32 {$$=v_float;} | BOOL {$$=v_bool;} | STRING {$$=v_string;} ;

Parameters: ID Type ParamOpts {$$=create_param($1, $2, $3);} ;

ParamOpts: ParamOpts COMMA ID Type {$$=create_param($3, $4, $1);} | /* empty */ {$$=NULL;} ;

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
}