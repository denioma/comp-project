#pragma once

typedef enum { v_void, v_int, v_float, v_bool, v_string } v_type;

typedef struct {
    v_type typespec;
    char* id;
} var_dec;

typedef enum {
    nop, op_or, op_and, op_eq, op_lt, op_gt,
    op_le, op_ge, op_add, op_ne, op_sub, 
    op_mul, op_div, op_mod, op_not, 
    op_minus, op_plus, op_call
} operation;

typedef struct _expr {
    operation operator;
} expr;

typedef struct _assign_stmt {
    char* id;
    expr* expression;
} assign_stmt;

typedef struct _print_stmt {
    char* strlit;
    expr* expression;
} print_stmt;

typedef struct _parse_args {
    char* id;
    int index;
} parse_args;

typedef struct _if_stmt {
    expr* condition;
    struct _stmt_dec* block1;
    struct _stmt_dec* block2;
} if_stmt;

typedef struct _for_stmt {
    expr* expression;
    struct _stmt_dec* block;
} for_stmt;

typedef enum { s_block, s_if, s_for, s_return, s_call, s_print, s_parse, s_assign } s_type;

typedef struct _stmt_dec {
    s_type type;
    union {
        parse_args* d_args;
        print_stmt* d_print;
        assign_stmt* d_assign;
        if_stmt* d_if;
        struct _stmt_dec* d_block;
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
    char* id;
    struct _param_dec* next;
} param_dec;

// function header node
typedef struct _func_header {
    char* id;
    v_type typespec;
    param_dec* param;
} func_header;

// function declaration node
typedef struct _func_dec {
    func_header* f_header;
    func_body* f_body;
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
