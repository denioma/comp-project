#pragma once

typedef enum {v_void, v_int, v_float, v_bool, v_string} v_type;

typedef struct _func_params func_params;

typedef struct _func_body func_body;

typedef struct _func_dec {
    char* id;
    v_type return_type;
    func_params* params;
    func_body* body;
} func_dec;

typedef struct {
    v_type typespec;
    char* id;
} var_dec;

typedef enum { d_var, d_func } d_type;

typedef struct _dec_node {
    d_type type;
    union {
        func_dec* func;
        var_dec* var;
    } dec;
    struct _dec_node* next;
} dec_node;

typedef struct _prog_node{
    dec_node* dlist;
} prog_node;

