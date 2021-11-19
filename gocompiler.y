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
    stmt_dec* stmt;
    expr* expression;
    stmt_block* s_block;
    func_invoc* s_call;
    f_invoc_opts* fi_opts;
}

%token <token> STRLIT ID INTLIT REALLIT
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
%type <decl> Declarations IdReps VarSpec VarDeclaration
%type <var_type> Type FuncType
%type <func> FuncDeclaration
%type <params> FuncParams Parameters ParamOpts
%type <f_body> VarsAndStatements VASOpts FuncBody
%type <stmt> Statement Stmt ParseArgs
%type <expression> Expr ExprOpt
%type <s_block> StmtBlock ExplicitBlock ElseStmt
%type <s_call> FuncInvocation
%type <fi_opts> FuncInvocationOpts ExprReps


%right ASSIGN
%left OR
%left AND
%left EQ NE GE GT LE LT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UNARY
%nonassoc LPAR RPAR
%%

Program: 
        PACKAGE ID SEMICOLON Declarations               {if (build) $$=program=new_prog($4);} 
;

Declarations:
        Declarations VarDeclaration SEMICOLON           {if (build) $$=insert_var_dec_list($1, $2);}
    |   Declarations FuncDeclaration SEMICOLON          {if (build) $$=insert_func_dec($1, $2);}
    |   /* empty */                                     {$$=NULL;}
;

VarDeclaration: 
        VAR VarSpec                                     {if (build) $$=$2;}
    |   VAR LPAR VarSpec SEMICOLON RPAR                 {if (build) $$=$3;}
;

FuncDeclaration: 
        FUNC ID LPAR FuncParams RPAR FuncType FuncBody  {if (build) $$=create_func($2, $6, $4, $7);}
;

FuncParams: 
        Parameters                                      {if (build) $$=$1;}
    |   /* empty */                                     {$$=NULL;}
;

FuncType: 
        Type                                            {if (build) $$=$1;}
    |   /* empty */                                     {if (build) $$=v_void;}
;

VarSpec: 
        ID IdReps Type                                  {if (build) $$=set_id_reps_head($2, $1, $3);}
;

IdReps: 
        IdReps COMMA ID                                 {if (build) $$=save_id_reps($1,$3);} 
    |   /* empty */                                     {$$=NULL;} 
;
        
Type: 
        INT                                             {if (build) $$=v_int;}
    |   FLOAT32                                         {if (build) $$=v_float;}
    |   BOOL                                            {if (build) $$=v_bool;}
    |   STRING                                          {if (build) $$=v_string;}
;

Parameters: 
        ID Type ParamOpts                               {if (build) $$=create_param($1, $2, $3);}
;

ParamOpts: 
        COMMA ID Type ParamOpts                         {if (build) $$=create_param($2, $3, $4);}
    |   /* empty */                                     {$$=NULL;}
;

FuncBody: 
        LBRACE VarsAndStatements RBRACE                 {if (build) $$=$2;}
;

VarsAndStatements: 
        VarsAndStatements VASOpts SEMICOLON             {if (build) $$=insert_to_body($2, $1);}
    |   /* empty */                                     {$$=NULL;}
;

VASOpts: 
        VarDeclaration                                  {if (build) $$=create_body_var($1);} 
    |   Statement                                       {if (build) $$=create_body_stmt($1);}
    |   /* empty */                                     {$$=NULL;}
;

Statement:
        ID ASSIGN Expr                                  {if (build) $$=create_assign($1, $3);}
    |   LBRACE Stmt RBRACE                              {if (build) $$=$2;}
    |   IF Expr LBRACE ExplicitBlock RBRACE ElseStmt    {if (build) $$=create_if($2, $4, $6);}
    |   FOR ExprOpt LBRACE ExplicitBlock RBRACE         {if (build) $$=create_for($2, $4);}
    |   RETURN ExprOpt                                  {if (build) $$=create_return($2);}
    |   FuncInvocation                                  {if (build) $$=create_call($1);}
    |   ParseArgs                                       {if (build) $$=$1;}
    |   PRINT LPAR STRLIT RPAR                          {if (build) $$=create_print($3, NULL);}
    |   PRINT LPAR Expr RPAR                            {if (build) $$=create_print(NULL, $3);}
    |   error                                           {$$=NULL;}
;

ExplicitBlock:
        Statement SEMICOLON ExplicitBlock               {if (build) $$=create_block($3, $1);}
    |   /* empty */                                     {$$=NULL;}
;

Stmt:
        Statement SEMICOLON                             {if (build) $$=$1;}
    |   StmtBlock                                       {if (build) $$=create_stmt_block_nullable($1);}
    |   /* empty */                                     {$$=NULL;}
;

StmtBlock: 
        Statement SEMICOLON StmtBlock                   {if (build) $$=block_or_null($3, $1);}
    |   Statement SEMICOLON Statement SEMICOLON         {if (build) $$=block_or_null(block_or_null(NULL, $3), $1);}
;

ElseStmt: 
        ELSE LBRACE ExplicitBlock RBRACE                {if (build) $$=$3;}
    |   /* empty */                                     {$$=NULL;}
;

ExprOpt: 
        Expr                                            {if (build) $$=$1;}
    |   /* empty */                                     {$$=NULL;} // $$=NULL;
;

ParseArgs: 
        ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR     {if (build) $$=create_pargs($1, $9);}
    |   ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ error RSQ RPAR    {$$=NULL;} 
;

FuncInvocation: 
        ID LPAR FuncInvocationOpts RPAR                 {if (build) $$=create_func_invocation($1,$3);}
    |   ID LPAR error RPAR                              {$$=NULL;}
;

FuncInvocationOpts: 
        Expr ExprReps                                   {if (build) $$=create_fi_opts($2, $1);}
    |   /* empty */                                     {$$=NULL;}
;
    
ExprReps: 
        COMMA Expr ExprReps                             {if (build) $$=create_fi_opts($3, $2);}
    |   /* empty */                                     {$$=NULL;}
;

Expr:
        Expr OR Expr                                    {if (build) $$=create_expr(e_expr, op_or, $1, $3);}
    |   Expr AND Expr                                   {if (build) $$=create_expr(e_expr, op_and, $1, $3);}
    |   Expr EQ Expr                                    {if (build) $$=create_expr(e_expr, op_eq, $1, $3);}
    |   Expr NE Expr                                    {if (build) $$=create_expr(e_expr, op_ne, $1, $3);}
    |   Expr LT Expr                                    {if (build) $$=create_expr(e_expr, op_lt, $1, $3);}
    |   Expr GT Expr                                    {if (build) $$=create_expr(e_expr, op_gt, $1, $3);}
    |   Expr GE Expr                                    {if (build) $$=create_expr(e_expr, op_ge, $1, $3);}
    |   Expr LE Expr                                    {if (build) $$=create_expr(e_expr, op_le, $1, $3);}
    |   Expr PLUS Expr                                  {if (build) $$=create_expr(e_expr, op_add, $1, $3);}
    |   Expr MINUS Expr                                 {if (build) $$=create_expr(e_expr, op_sub, $1, $3);}
    |   Expr STAR Expr                                  {if (build) $$=create_expr(e_expr, op_mul, $1, $3);}
    |   Expr DIV Expr                                   {if (build) $$=create_expr(e_expr, op_div, $1, $3);}
    |   Expr MOD Expr                                   {if (build) $$=create_expr(e_expr, op_mod, $1, $3);}
    |   NOT Expr                                        {if (build) $$=create_expr(e_expr, op_not, $2, NULL);}
    |   MINUS Expr %prec UNARY                          {if (build) $$=create_expr(e_expr, op_minus, $2, NULL);}
    |   PLUS Expr  %prec UNARY                          {if (build) $$=create_expr(e_expr, op_plus, $2, NULL);}
    |   INTLIT                                          {if (build) $$=create_expr(e_int, nop, $1, NULL);}
    |   REALLIT                                         {if (build) $$=create_expr(e_real, nop, $1, NULL);}
    |   ID                                              {if (build) $$=create_expr(e_id, nop, $1, NULL);}
    |   FuncInvocation                                  {if (build) $$=create_expr(e_func, nop, $1, NULL);}
    |   LPAR Expr RPAR                                  {if (build) $$=$2;}
    |   LPAR error RPAR                                 {$$=NULL;}
;
%%
void yyerror(const char* s) {
    printf("Line %d, column %d: %s: %s\n", yline, ycol, s, yytext);
    build = 0;
}
