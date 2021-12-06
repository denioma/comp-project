#pragma once
#include "symtab.h"

typedef struct _token {
    char* value;
    int line;
    int col; 
} token;

typedef enum { v_int, v_float, v_bool, v_string, v_void } v_type;

typedef struct {
    v_type typespec;
    token* tkn;
} var_dec;

typedef enum {
    op_add, op_and, op_call, op_div,
    op_eq, op_ge, op_gt, op_le, op_lt, 
    op_minus, op_mod, op_mul, op_ne, 
    op_not, op_or, op_plus, op_sub, nop
} op;

typedef enum { e_int, e_real, e_expr, e_func, e_id } e_type;

typedef struct _expr {
    e_type type;
    op operator;
    token* tkn;
    t_type annotation;
    union {
        struct _expr* exp_1;
        struct _func_invoc* call;
    } arg1;
    struct _expr* arg2;
} expr;

typedef struct _assign_stmt {
    token* tkn;
    token* var;
    t_type type;
    expr* expression;
} assign_stmt;

typedef struct _print_stmt {
    token* tkn;
    char* strlit;
    expr* expression;
} print_stmt;

typedef struct _parse_args {
    token* tkn;
    token* var;
    t_type type;
    expr* index;
} parse_args;

typedef struct _if_stmt {
    expr* condition;
    struct _stmt_dec* block1;
    struct _stmt_dec* block2;
} if_stmt;

typedef struct _for_stmt {
    expr* condition;
    struct _stmt_dec* block;
} for_stmt;

typedef enum { s_block, s_if, s_for, s_return, s_call, s_print, s_parse, s_assign } s_type;

typedef struct _stmt_block {
    struct _stmt_dec* stmt;
    struct _stmt_block* next;
} stmt_block;

typedef struct _f_invoc_opts {
    expr* opt;
    struct _f_invoc_opts* next;
} f_invoc_opts;

typedef struct _func_invoc {
    token* tkn;
    t_type annotation;
    f_params* params;
    f_invoc_opts* opts;
} func_invoc;

typedef struct _stmt_dec {
    token *tkn;
    s_type type;
    union {
        parse_args* d_args;
        print_stmt* d_print;
        assign_stmt* d_assign;
        if_stmt* d_if;
        for_stmt* d_for;
        expr* d_expr;
        stmt_block* d_block;
        func_invoc* d_fi;
    } dec;
} stmt_dec;

typedef enum { b_var, b_stmt } b_type;

typedef struct _func_body {
    b_type type;
    union {
        var_dec* var;
        stmt_dec* stmt;
    } dec;
    struct _func_body* next;
} func_body;

// function parameter declaration node
typedef struct _param_dec {
    v_type typespec;
    token* tkn;
    struct _param_dec* next;
} param_dec;

// function header node
typedef struct _func_header {
    token* tkn;
    v_type typespec;
    param_dec* param;
} func_header;

// function declaration node
typedef struct _func_dec {
    func_header* f_header;
    func_body* f_body;
    symtab* localsym;
} func_dec;

typedef enum { d_var, d_func } d_type;

typedef struct _dec_node {
    d_type type;
    union {
        func_dec* func;
        var_dec* var;
    } dec;
    struct _dec_node* next;
} dec_node;

typedef struct _prog_node {
    dec_node* dlist;
} prog_node;
