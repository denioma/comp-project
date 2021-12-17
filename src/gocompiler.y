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
    token* tkn;
    dec_node* decl;
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

%token <tkn> STRLIT ID INTLIT REALLIT
%token <tkn> PLUS MINUS STAR DIV MOD EQ GE GT LE LT NE NOT AND OR RETURN PRINT PARSEINT INT FLOAT32 BOOL STRING ASSIGN
%token PACKAGE IF ELSE FOR LPAR RPAR LBRACE RBRACE LSQ RSQ SEMICOLON COMMA BLANKID VAR CMDARGS FUNC
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

/* %destructor {if (!build) destroy_tkn($$);} <tkn>
%destructor {if (!build) destroy_dec($$);} <decl>
%destructor {if (!build) destroy_func($$);} <func>
%destructor {if (!build) destroy_func_params($$);} <params>
%destructor {if (!build) destroy_func_body($$);} <f_body>
%destructor {if (!build) destroy_stmt_dec($$);} <stmt>
%destructor {if (!build) destroy_expr($$);} <expression>
%destructor {if (!build) destroy_stmt_block($$);} <s_block>
%destructor {if (!build) destroy_func_invoc($$);} <s_call>
%destructor {if (!build) destroy_func_invoc_opts($$);} <fi_opts> */

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
        PACKAGE ID SEMICOLON Declarations               {destroy_tkn($2); if (build) $$=program=new_prog($4);} 
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
        ID ASSIGN Expr                                  {if (build) $$=create_assign($2, $1, $3);}
    |   LBRACE Stmt RBRACE                              {if (build) $$=$2;}
    |   IF Expr LBRACE ExplicitBlock RBRACE ElseStmt    {if (build) $$=create_if($2, $4, $6);}
    |   FOR ExprOpt LBRACE ExplicitBlock RBRACE         {if (build) $$=create_for($2, $4);}
    |   RETURN ExprOpt                                  {if (build) $$=create_return($1, $2);}
    |   FuncInvocation                                  {if (build) $$=create_call($1);}
    |   ParseArgs                                       {if (build) $$=$1;}
    |   PRINT LPAR STRLIT RPAR                          {if (build) $$=create_print($1, $3, NULL);}
    |   PRINT LPAR Expr RPAR                            {if (build) $$=create_print($1, NULL, $3);}
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
        ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR     {if (build) $$=create_pargs($5, $1, $9); destroy_tkn($4);}
    |   ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ error RSQ RPAR    {$$=NULL; destroy_tkn($4);} 
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
        Expr OR Expr                                    {if (build) $$=create_expr(e_expr, op_or, $2, $1, $3);}
    |   Expr AND Expr                                   {if (build) $$=create_expr(e_expr, op_and, $2, $1, $3);}
    |   Expr EQ Expr                                    {if (build) $$=create_expr(e_expr, op_eq, $2, $1, $3);}
    |   Expr NE Expr                                    {if (build) $$=create_expr(e_expr, op_ne, $2, $1, $3);}
    |   Expr LT Expr                                    {if (build) $$=create_expr(e_expr, op_lt, $2, $1, $3);}
    |   Expr GT Expr                                    {if (build) $$=create_expr(e_expr, op_gt, $2, $1, $3);}
    |   Expr GE Expr                                    {if (build) $$=create_expr(e_expr, op_ge, $2, $1, $3);}
    |   Expr LE Expr                                    {if (build) $$=create_expr(e_expr, op_le, $2, $1, $3);}
    |   Expr PLUS Expr                                  {if (build) $$=create_expr(e_expr, op_add, $2, $1, $3);}
    |   Expr MINUS Expr                                 {if (build) $$=create_expr(e_expr, op_sub, $2, $1, $3);}
    |   Expr STAR Expr                                  {if (build) $$=create_expr(e_expr, op_mul, $2, $1, $3);}
    |   Expr DIV Expr                                   {if (build) $$=create_expr(e_expr, op_div, $2, $1, $3);}
    |   Expr MOD Expr                                   {if (build) $$=create_expr(e_expr, op_mod, $2, $1, $3);}
    |   NOT Expr                                        {if (build) $$=create_expr(e_expr, op_not, $1, $2, NULL);}
    |   MINUS Expr %prec UNARY                          {if (build) $$=create_expr(e_expr, op_minus, $1, $2, NULL);}
    |   PLUS Expr  %prec UNARY                          {if (build) $$=create_expr(e_expr, op_plus, $1, $2, NULL);}
    |   INTLIT                                          {if (build) $$=create_expr(e_int, nop, NULL, $1, NULL);}
    |   REALLIT                                         {if (build) $$=create_expr(e_real, nop, NULL, $1, NULL);}
    |   ID                                              {if (build) $$=create_expr(e_id, nop, $1, $1, NULL);}
    |   FuncInvocation                                  {if (build) $$=create_expr(e_func, nop, NULL, $1, NULL);}
    |   LPAR Expr RPAR                                  {if (build) $$=$2;}
    |   LPAR error RPAR                                 {$$=NULL;}
;
%%
void yyerror(const char* s) {
    printf("Line %d, column %d: %s: %s\n", yline, ycol, s, yytext);
    build = 0;
}
