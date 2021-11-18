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
%type <stmt> Statement Stmt ElseStmt ParseArgs
%type <expression> Expr ExprOpt
%type <s_block> StmtBlock
%type <s_call> FuncInvocation
%type <fi_opts> FuncInvocationOpts ExprReps


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

Program: 
        PACKAGE ID SEMICOLON Declarations               {$$=program=new_prog($4);} 
;

Declarations:
        Declarations VarDeclaration SEMICOLON           {$$=insert_var_dec_list($1, $2);}
    |   Declarations FuncDeclaration SEMICOLON          {$$=insert_func_dec($1, $2);}
    |   /* empty */                                     {$$=NULL;}
;

VarDeclaration: 
        VAR VarSpec                                     {$$=$2;}
    |   VAR LPAR VarSpec SEMICOLON RPAR                 {$$=$3;}
;

FuncDeclaration: 
        FUNC ID LPAR FuncParams RPAR FuncType FuncBody  {$$=create_func($2, $6, $4, $7);}
;

FuncParams: 
        Parameters                                      {$$=$1;}
    |   /* empty */                                     {$$=NULL;}
;

FuncType: 
        Type                                            {$$=$1;}
    |   /* empty */                                     {$$=v_void;}
;

VarSpec: 
        ID IdReps Type                                  {$$=set_id_reps_head($2, $1, $3);}
;

IdReps: 
        IdReps COMMA ID                                 {$$=save_id_reps($1,$3);} 
    |   /* empty */                                     {$$=NULL;} 
;
        
Type: 
        INT                                             {$$=v_int;}
    |   FLOAT32                                         {$$=v_float;}
    |   BOOL                                            {$$=v_bool;}
    |   STRING                                          {$$=v_string;}
;

Parameters: 
        ID Type ParamOpts                               {$$=create_param($1, $2, $3);}
;

ParamOpts: 
        ParamOpts COMMA ID Type                         {$$=create_param($3, $4, $1);}
    |   /* empty */                                     {$$=NULL;}
;

FuncBody: 
        LBRACE VarsAndStatements RBRACE                 {$$=$2;}
;

VarsAndStatements: 
        VarsAndStatements VASOpts SEMICOLON             {$$=insert_to_body($2, $1);}
    |   /* empty */                                     {$$=NULL;}
;

VASOpts: 
        VarDeclaration                                  {$$=create_body_var($1);} 
    |   Statement                                       {$$=create_body_stmt($1);}
    |   /* empty */                                     {$$=NULL;}
;

Statement:
        ID ASSIGN Expr                                  {$$=create_assign($1, $3);}
    |   LBRACE Stmt RBRACE                              {$$=$2;}
    |   IF Expr LBRACE Stmt RBRACE ElseStmt             {$$=create_if($2, $4, $6);}
    |   FOR ExprOpt LBRACE Stmt RBRACE                  {$$=create_for($2, $4);}
    |   RETURN ExprOpt                                  {$$=create_return($2);}
    |   FuncInvocation                                  {$$=create_call($1);}
    |   ParseArgs                                       {$$=$1;}
    |   PRINT LPAR STRLIT RPAR                          {$$=create_print($3, NULL);}
    |   PRINT LPAR Expr RPAR                            {$$=create_print(NULL, $3);}
    |   error                                           {$$=NULL;}
;

Stmt:
        /* empty */                                     {$$=NULL;}
    |   Statement SEMICOLON                             {$$=$1;}
    |   StmtBlock                                       {$$=create_stmt_block($1);}
;

StmtBlock: 
        /* TODO create stmt_dec* chain */
        StmtBlock Statement SEMICOLON                   {$$=add_block_stmt($1, $2);}
    |   Statement SEMICOLON Statement SEMICOLON         {$$=create_block(create_block(NULL, $3), $1);}
;

ElseStmt: 
        ELSE LBRACE Stmt RBRACE                         {$$=$3;}
    |   /* empty */                                     {$$=NULL;}
;

ExprOpt: 
        Expr                                            {$$=$1;}
    |   /* empty */                                     {$$=NULL;} // $$=NULL;
;

ParseArgs: 
        ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR     {$$=create_pargs($1, $9);}
    |   ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ error RSQ RPAR    {$$=NULL;} 
;

FuncInvocation: 
        ID LPAR FuncInvocationOpts RPAR                 {$$=create_func_invocation($1,$3);}
    |   ID LPAR error RPAR                              {$$=NULL;}
;

FuncInvocationOpts: 
        Expr ExprReps                                   {$$=create_fi_opts($1, NULL);}
    |   /* empty */                                     {$$=NULL;}
;
    
ExprReps: 
        ExprReps COMMA Expr                             {$$=NULL;}
    |   /* empty */                                     {$$=NULL;}
;

Expr:
        Expr PLUS Expr                                  {$$=create_expr(e_expr, op_add, $1, $3);}
    |   Expr MINUS Expr                                 {$$=create_expr(e_expr, op_sub, $1, $3);}
    |   Expr STAR Expr                                  {$$=create_expr(e_expr, op_mul, $1, $3);}
    |   Expr DIV Expr                                   {$$=create_expr(e_expr, op_div, $1, $3);}
    |   Expr MOD Expr                                   {$$=create_expr(e_expr, op_mod, $1, $3);}
    |   Expr OR Expr                                    {$$=create_expr(e_expr, op_or, $1, $3);}
    |   Expr AND Expr                                   {$$=create_expr(e_expr, op_and, $1, $3);}
    |   Expr LT Expr                                    {$$=create_expr(e_expr, op_lt, $1, $3);}
    |   Expr GT Expr                                    {$$=create_expr(e_expr, op_gt, $1, $3);}
    |   Expr EQ Expr                                    {$$=create_expr(e_expr, op_eq, $1, $3);}
    |   Expr GE Expr                                    {$$=create_expr(e_expr, op_ge, $1, $3);}
    |   Expr LE Expr                                    {$$=create_expr(e_expr, op_le, $1, $3);}
    |   Expr NE Expr                                    {$$=create_expr(e_expr, op_ne, $1, $3);}
    |   NOT Expr                                        {$$=create_expr(e_expr, op_not, $2, NULL);}
    |   MINUS Expr                                      {$$=create_expr(e_expr, op_minus, $2, NULL);}
    |   PLUS Expr                                       {$$=create_expr(e_expr, op_plus, $2, NULL);}
    |   INTLIT                                          {$$=create_expr(e_int, nop, $1, NULL);}
    |   REALLIT                                         {$$=create_expr(e_real, nop, $1, NULL);}
    |   ID                                              {$$=create_expr(e_id, nop, $1, NULL);}
    |   FuncInvocation                                  {$$=create_expr(e_func, nop, $1, NULL);}
    |   LPAR Expr RPAR                                  {$$=$2;}
    |   LPAR error RPAR                                 {$$=NULL;}
;
%%
void yyerror(const char* s) {
    printf("Line %d, column %d: %s: %s\n", yylineno, col, s, yytext);
}